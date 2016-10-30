/*
* TREAD LIGHTLY
* Authors: Tom Chapple (29/10)
* File: PathFinder.h
* Last modified on: 29/10/16
* Purpose: Describes the class used to represent
* pathfinding through a network of node. In this
* case, it uses MapData.
*/

#pragma once

#include <vector>
#include "MapData.h"

namespace TreadLightly {

	namespace MapUtilities {

		class PathFinder {

		public:
			/* ~~~ Types / Typedefs ~~~ */
			typedef float Weight;
			struct Position {
				pos_type x;
				pos_type y;

				Position();
				Position(const std::pair<pos_type, pos_type>& a);
				~Position();
				Position& operator=(const std::pair<pos_type, pos_type>& right);
			};

			struct TraverseNode {
				Cell ThisCell;
				Cell FromCell;
				Weight DistanceTravelled;
				Weight DistanceEstimate;

				TraverseNode();
				~TraverseNode();
			};

			/* ~~~ Comparison Function Objects ~~~ */
			struct TraverseHeapComp_t {
				bool operator()(const TraverseNode& x, const TraverseNode& y) const;
				typedef TraverseNode first_argument_type;
				typedef TraverseNode second_argument_type;
				typedef bool result_type;
			} TraverseHeapComp;

			struct TraverseSetComp_t {
				bool operator()(const TraverseNode& left, const TraverseNode& right) const;
				typedef TraverseNode first_argument_type;
				typedef TraverseNode second_argument_type;
				typedef bool result_type;
			} TraverseSetComp;

			/* ~~~ Constructors / Destructors ~~~ */
			PathFinder(const Data& mapData);
			~PathFinder();

			/* ~~~ Interface Methods ~~~ */
			bool FindPath(const Cell& from, const Cell& to, std::vector<Cell>& store);

			bool FindPath(const std::pair<pos_type, pos_type>& from,
				const std::pair<pos_type, pos_type>& to,
				std::vector<Cell>& store);

			bool FindPath(pos_type fromX, pos_type fromY,
				pos_type toX, pos_type toY,
				std::vector<Cell>& store);

			void SetData(const Data& mapData);
			const Data *GetData() const;

		protected:

			/* ~~~ Members ~~~ */
			const Data *_Data;

			/* ~~~ Internal Functions ~~~ */
			inline Weight _EstimateDistance(const Cell& from, const Cell& to);
			bool _VisitNextNode(const Cell& dest,
				std::vector<TraverseNode>& consider,
				std::set<TraverseNode, TraverseSetComp_t>& visited);
			bool _ConsiderNode(const Cell& newPos, const Weight& distance,
				const TraverseNode& from,
				const Cell& dest,
				std::vector<TraverseNode>& consider,
				std::set<TraverseNode, TraverseSetComp_t>& visited);
			void _AddAdjacentNodes(const TraverseNode& node,
				const Cell& dest,
				std::vector<TraverseNode>& consider,
				std::set<TraverseNode, TraverseSetComp_t>& visited);


		};
	}
}