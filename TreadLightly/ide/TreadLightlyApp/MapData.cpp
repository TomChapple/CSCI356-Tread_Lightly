#include "stdafx.h"

#include <cmath>

#include "map/MapData.h"

namespace TreadLightly {

	namespace MapUtilities {

		Cell::Cell(pos_type x, pos_type y, Zone zone, Team team, Traverse trav):
			_X(x), _Y(y), _Zone(zone), _Team(team), _Traverse(trav) {

		}

		Cell::Cell(const Cell& x):
			_X(x._X), _Y(x._Y), _Zone(x._Zone),
			_Team(x._Team), _Traverse(x._Traverse) {

		}

		Cell::~Cell() {

		}

		Cell& Cell::operator=(const Cell& right) {
			_X = right._X;
			_Y = right._Y;
			_Zone = right._Zone;
			_Team = right._Team;
			_Traverse = right._Traverse;
		}

		pos_type Cell::GetX() const {
			return _X;
		}

		pos_type Cell::GetY() const {
			return _Y;
		}

		std::pair<pos_type, pos_type> Cell::GetPoint() const {
			return std::make_pair(_X, _Y);
		}

		Cell::Zone Cell::GetZone() const {
			return _Zone;
		}

		Cell::Team Cell::GetTeam() const {
			return _Team;
		}

		Cell::Traverse Cell::GetTraverseType() const {
			return _Traverse;
		}

		bool Cell::IsTraversable() const {
			return _Traverse == TRAVERSE_FREE;
		}

		Data::iterator::iterator():
			_Ptr(NULL), _Offset(0), _MaxOffset(0) {

		}

		Data::iterator::iterator(const iterator& x):
			_Ptr(x._Ptr), _Offset(x._Offset), _MaxOffset(x._MaxOffset) {

		}

		Data::iterator::~iterator() {

		}

		Data::iterator& Data::iterator::operator=(const iterator& right) {
			_Ptr = right._Ptr;
			_Offset = right._Offset;
			_MaxOffset = right._MaxOffset;
		}

		Cell Data::iterator::operator*() const {
			if (_ValidateIterator())
				return *_Ptr;
			else throw std::out_of_range("MapData iterator cannot be dereferenced");
		}

		Cell *Data::iterator::operator->() const {
			if (_ValidateIterator())
				return _Ptr;
			else throw std::out_of_range("MapData iterator cannot be dereferenced");
		}

		Data::iterator& Data::iterator::operator++() {
			if (_ValidateIterator()) {
				++_Ptr;
			}
			++_Offset;
			return *this;
		}

		Data::iterator Data::iterator::operator++(int) {
			iterator ReturnIt(*this);
			if (_ValidateIterator()) {
				++_Ptr;
			}
			++_Offset;
			return ReturnIt;
		}

		Data::iterator& Data::iterator::operator--() {
			if (_ValidateIterator()) {
				--_Ptr;
			}
			--_Offset;
			return *this;
		}

		Data::iterator Data::iterator::operator--(int) {
			iterator ReturnIt(*this);
			if (_ValidateIterator()) {
				--_Ptr;
			}
			--_Offset;
			return ReturnIt;
		}

		inline bool Data::iterator::_ValidateIterator() const {
			return (_Ptr != NULL) && (_Offset < _MaxOffset) &&
				(_Offset >= 0);
		}

		bool Data::iterator::operator==(const iterator& right) const {
			return (_Ptr == right._Ptr) && (_Offset == right._Offset);
		}

		bool Data::iterator::operator==(const iterator& right) const {
			return (_Ptr != right._Ptr) || (_Offset != right._Offset);
		}

		Data::Data():
			_HasData(false), _Data(NULL), _Width(0), _Height(0) {

		}

		Data::Data(const std::string& path):
			_HasData(false), _Data(NULL), _Width(0), _Height(0) {

			LoadFromFile(path);
		}

		Data::~Data() {
			if (_Data)
				delete[] _Data;
		}

		pos_type Data::GetWidth() const {
			return _Width;
		}

		pos_type Data::GetHeight() const {
			return _Height;
		}

		Data::const_iterator Data::begin() const {
			if (!HasData())
				throw std::runtime_error("No data in MapData");

			iterator ReturnIt;
			ReturnIt._MaxOffset = _Width * _Height;
			ReturnIt._Offset = 0;
			ReturnIt._Ptr = _Data;

			return ReturnIt;
		}

		Data::const_iterator Data::end() const {
			if (!HasData())
				throw std::runtime_error("No data in MapData");

			iterator ReturnIt;
			ReturnIt._MaxOffset = _Width * _Height;
			ReturnIt._Offset = ReturnIt._MaxOffset;
			ReturnIt._Ptr = _Data + ReturnIt._Offset - 1;

			return ReturnIt;
		}

		Cell Data::GetCell(pos_type x, pos_type y) const {
			if (!HasData())
				throw std::runtime_error("No data in MapData");

			return *(_Data + x + y * _Width);
		}

		template <typename Container>
		void Data::GetAdjacentCells(axis_t x, axis_t y, Container<Cell>& store) const {

			pos_type WidthOffset = 1,
				HeightOffset = _Width;

			Cell *CurrCell = _Data + x + y * HeightOffset;

			/* Check directly adjacent cells */
			bool TopEdge = (y > 0), BotEdge = (y < _Height),
				LeftEdge = (x > 0), RightEdge = (x < _Width);
			bool TopBlocked = true, BotBlocked = true,
				LeftBlocked = true, RightBlocked = true;

			// Left
			CurrCell -= WidthOffset;
			if (LeftEdge && CurrCell->IsTraversable()) {
				LeftBlocked = false;
				store.insert(store.begin(), *CurrCell);
			}

			// Right
			CurrCell += 2 * WidthOffset;
			if (RightEdge && CurrCell->IsTraversable()) {
				RightBlocked = false;
				store.insert(store.begin(), *CurrCell);
			}

			// Bottom
			CurrCell += HeightOffset - WidthOffset;
			if (BotEdge && CurrCell->IsTraversable()) {
				BotBlocked = false;
				store.insert(store.begin(), *CurrCell);
			}

			// Top
			CurrCell -= 2 * HeightOffset;
			if (TopEdge && CurrCell->IsTraversable()) {
				TopBlocked = false;
				store.insert(store.begin(), *CurrCell);
			}

			/* Now do diagonals based on previous results */

			// Top-Left
			CurrCell -= WidthOffset;
			if (!TopBlocked && !LeftBlocked && CurrCell->IsTraversable())
				store.insert(store.begin(), *CurrCell);

			// Top-Right
			CurrCell += 2 * WidthOffset;
			if (!TopBlocked && !RightBlocked && CurrCell->IsTraversable())
				store.insert(store.begin(), *CurrCell);

			// Bottom-Right
			CurrCell += 2 * HeightOffset;
			if (!BotBlocked && !RightBlocked && CurrCell->IsTraversable())
				store.insert(store.begin(), *CurrCell);

			// Bottom-Left
			CurrCell -= 2 * WidthOffset;
			if (!BotBlocked && !LeftBlocked && CurrCell->IsTraversable())
				store.insert(store.begin(), *CurrCell);

		}

		float Data::Heuristic(const Cell& from, const Cell& to) const {
			return sqrtf(powf(from.GetX() - to.GetX(), 2) +
				powf(from.GetY() - to.GetY(), 2));
		}

		void Data::LoadFromFile(const std::string& filename) {
			// Need to finalise how the file is to be loaded.
		}

		bool Data::HasData() const {
			return (_Data == NULL);
		}
	}
}