/*
* TREAD LIGHTLY
* Authors: Tom Chapple (29/10)
* File: PathFinder.cpp
* Last modified on: 29/10/16
* Purpose: Implements the PathFinder utility class
* for our main Map class.
*/
#include "stdafx.h"
#include "map/PathFinder.h"

#include <cmath>

const TreadLightly::MapUtilities::PathFinder::Weight WEIGHT_STRAIGHT = 1.0f,
	WEIGHT_DIAGONAL = 1.414213f;

namespace TreadLightly {

	namespace MapUtilities {

		PathFinder::Position::Position():
			x(0), y(0) {

		}

		PathFinder::Position::Position(const std::pair<pos_type, pos_type>& a):
			x(a.first), y(a.second) {

		}

		PathFinder::Position::~Position() {

		}

		PathFinder::Position& PathFinder::Position::operator=(const std::pair<pos_type, pos_type>& right) {
			x = right.first; y = right.second;
			return *this;
		}

		PathFinder::TraverseNode::TraverseNode():
			ThisCell(0, 0, (Cell::Zone)0, (Cell::Team)0, (Cell::Traverse)0),
			FromCell(0, 0, (Cell::Zone)0, (Cell::Team)0, (Cell::Traverse)0) {

		}

		PathFinder::TraverseNode::~TraverseNode() {

		}

		bool PathFinder::TraverseHeapComp_t::operator()(const TraverseNode& x, const TraverseNode& y) const {
			return x.DistanceEstimate > y.DistanceEstimate;
		}

		bool PathFinder::TraverseSetComp_t::operator()(const TraverseNode& left, const TraverseNode& right) const {
			return (left.ThisCell.GetY() < right.ThisCell.GetY()) ||
				((left.ThisCell.GetY() == right.ThisCell.GetY()) &&
				(left.ThisCell.GetX() < right.ThisCell.GetX()));
		}

		PathFinder::PathFinder(const Data& mapData):
			_Data(&mapData) {

		}

		PathFinder::~PathFinder() {

		}

		bool PathFinder::FindPath(const Cell& from, const Cell& to,
			std::vector<Cell>& store) {

			return FindPath(from.GetX(), from.GetY(),
				to.GetX(), to.GetY(),
				store);
		}

		bool PathFinder::FindPath(const std::pair<pos_type, pos_type>& from,
			const std::pair<pos_type, pos_type>& to,
			std::vector<Cell>& store) {

			return FindPath(from.first, from.second,
				to.first, to.second,
				store);
		}

		bool PathFinder::FindPath(pos_type fromX, pos_type fromY,
			pos_type toX, pos_type toY,
			std::vector<Cell>& store) {

			std::vector<Position> AdjacentPositions;
			AdjacentPositions.reserve(8);

			Cell FromCell = _Data->GetCell(fromX, fromY),
				ToCell = _Data->GetCell(toX, toY);

			/* Check if our to or from positions are valid before we begin */
			if (!FromCell.IsTraversable() ||
				!ToCell.IsTraversable())
				return false;

			/* If they are, setup for our traversal */
			//weight_t *Heuristics = new weight_t[Elements]; // Used to hold heuristics as we go instead of calculating them all

			std::set<TraverseNode, TraverseSetComp_t> VisitedSet;
			std::vector<TraverseNode> ConsiderSet;

			TraverseNode VisitingNode;
			VisitingNode.ThisCell = FromCell;
			VisitingNode.FromCell = FromCell;
			VisitingNode.DistanceTravelled = 0.0f;
			VisitingNode.DistanceEstimate = _EstimateDistance(FromCell, ToCell);

			ConsiderSet.push_back(VisitingNode);

			bool ReachedDestination = false;
			while (!ReachedDestination &&
				!ConsiderSet.empty()) {

				ReachedDestination =
					_VisitNextNode(ToCell, ConsiderSet, VisitedSet);
			}

			/* If it failed, return now */
			if (!ReachedDestination)
				return false;

			/* Store results according to storage iterator */
			TraverseNode StoreNode;
			StoreNode.ThisCell = ToCell;
			auto FindIt = VisitedSet.find(StoreNode);
			StoreNode = *FindIt;
			auto StoreIt = store.begin();

			/* Keep going until we point to ourselves */
			//while ((StoreNode.ThisPos.x != StoreNode.FromPos.x) ||
			//	(StoreNode.ThisPos.y != StoreNode.FromPos.y)) {
			while (StoreNode.ThisCell != StoreNode.FromCell) {

				StoreIt = store.insert(StoreIt, StoreNode.ThisCell);
				StoreNode.ThisCell = StoreNode.FromCell;

				FindIt = VisitedSet.find(StoreNode);
				StoreNode = *FindIt;
			}

			/* Add in the final node */
			store.insert(StoreIt, StoreNode.ThisCell);

			return true;
		}

		void PathFinder::SetData(const Data& mapData) {
			_Data = &mapData;
		}

		const Data *PathFinder::GetData() const {
			return _Data;
		}

		inline PathFinder::Weight PathFinder::_EstimateDistance(const Cell& from, const Cell& to) {
			return sqrtf(powf(from.GetX() - to.GetX(), 2) + powf(from.GetY() - to.GetY(), 2));
		}

		bool PathFinder::_VisitNextNode(const Cell& dest,
			std::vector<TraverseNode>& consider,
			std::set<TraverseNode, TraverseSetComp_t>& visited) {

			/* Get node from consider set and add to Visited Set */
			std::pop_heap(consider.begin(), consider.end(), TraverseHeapComp);
			TraverseNode VisitingNode = consider.back();
			consider.pop_back();

			/* Check if this is our destination */
			if (VisitingNode.ThisCell == dest) {

				// Jeez Tom, remember to add the damn destination
				visited.insert(VisitingNode);
				return true;
			}

			/*
			* Need to check incoming node to see if it has already
			* been visited.
			* If it has, pull it off Visited set, update it and put
			* it back on.
			*/
			bool AddToVisited = true;
			auto FindNode = visited.find(VisitingNode);
			if (FindNode != visited.end()) {

				/* Compare the two nodes */
				if (VisitingNode.DistanceTravelled < FindNode->DistanceTravelled) {

					// If less than, erase the current one and add the new one in
					FindNode = visited.erase(FindNode);
				}
				else {
					AddToVisited = false;
				}
			}

			if (AddToVisited) {
				visited.insert(VisitingNode);
				_AddAdjacentNodes(VisitingNode, dest, consider, visited);
			}

			return false;
		}

		void PathFinder:: _AddAdjacentNodes(const TraverseNode& node,
			const Cell& dest,
			std::vector<TraverseNode>& consider,
			std::set<TraverseNode, TraverseSetComp_t>& visited) {

			// Take advantage of the Data's function we have
			std::vector<Cell> ConsiderNodes;
			_Data->GetAdjacentCells(node.ThisCell.GetX(), node.ThisCell.GetY(), ConsiderNodes);

			// Go through each and add them in (with the appropriate weight)
			for (Cell& cell : ConsiderNodes) {
				Weight Distance;
				if ((cell.GetX() == node.ThisCell.GetX()) ||
					(cell.GetY() == node.ThisCell.GetY())) {
					Distance = WEIGHT_STRAIGHT;
				}
				else
					Distance = WEIGHT_DIAGONAL;

				_ConsiderNode(cell, Distance, node, dest, consider, visited);
			}
		}

		bool PathFinder::_ConsiderNode(const Cell& newPos, const Weight& distance,
			const TraverseNode& from,
			const Cell& dest,
			std::vector<TraverseNode>& consider,
			std::set<TraverseNode, TraverseSetComp_t>& visited) {

			bool Added;
			TraverseNode AddNode;

			/* Check if out of bounds */
			if ((newPos.GetY() < 0) || (newPos.GetY() >= _Data->GetHeight()) ||
				(newPos.GetX() < 0) || (newPos.GetX() >= _Data->GetWidth())) {
					Added = false;
			}
			/* Check if traversable */
			else if (!newPos.IsTraversable())
				Added = false;
			/* Check if the node being added was the previous node (still need it to be valid) */
			else if (from.FromCell == newPos)
				Added = true;
			else {
				/* Construct new node */
				AddNode.ThisCell = newPos;
				AddNode.FromCell = from.ThisCell;
				AddNode.DistanceTravelled = from.DistanceTravelled + 
					distance;
				AddNode.DistanceEstimate = AddNode.DistanceTravelled +
					_EstimateDistance(AddNode.ThisCell, dest);

				/*
				* Don't need to check the Consider Set as the Visiting Set
				* can just check whether it can be used anyway.
				* Just causes our heap to get a little larger, that's all.
				*/

				// Add Node to Heap
				consider.push_back(AddNode);
				std::push_heap(consider.begin(), consider.end(), TraverseHeapComp);

				Added = true;
			}
			
			return Added;
		}
	}
}