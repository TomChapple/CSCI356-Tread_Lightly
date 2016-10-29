#include "stdafx.h"

#include <cmath>

#define IMAGE_ALLOC OGRE_IMAGE
#ifdef IMAGE_ALLOC == OGRE_IMAGE
#include <OgreImage.h>
#endif

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

			return *this;
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

			return *this;
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

		bool Data::iterator::operator!=(const iterator& right) const {
			return (_Ptr != right._Ptr) || (_Offset != right._Offset);
		}

		Data::Data():
			_HasData(false), _Buffer(NULL), _Data(NULL), _Width(0), _Height(0) {

		}

		Data::Data(const std::string& path):
			_HasData(false), _Buffer(NULL), _Data(NULL), _Width(0), _Height(0) {

			LoadFromFile(path);
		}

		Data::~Data() {
			/*
			* Due to the hacky nature, need to destroy buffer in a specific
			* way
			*/
			_DestroyCellBuffer();
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

		void Data::GetAdjacentCells(pos_type x, pos_type y, std::vector<Cell>& store) const {

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

			/*
			* BIT ALLOCATION TABLE
			* -----------------------------------
			* RRRRRRRR GGGGGGGG BBBBBBBB AAAAAAAA
			*          bbbbzzzz      ttt hhhhhhhh
			* h (A1-8): Height Value (256 options, unused)
			* t (B1-3): Team Value (eight options)
			* z (G1-4): Zone Identifier (sixteen options)
			* b (G5-8): Traversal Type (sixteen options)
			*/
			const Ogre::uint32 HEIGHT_MASK = 0x000000FF,
				TEAM_MASK = 0x00000700,
				ZONE_MASK = 0x000F0000,
				TRAVERSE_MASK = 0x00F00000;
			const int HEIGHT_SHIFT = 0,
				TEAM_SHIFT = 8,
				ZONE_SHIFT = 16,
				TRAVERSE_SHIFT = 20;

			/* Load Data in from image file according to above table */
#ifdef IMAGE_ALLOC == OGRE_IMAGE
			Ogre::Image HeightMap;
			HeightMap.load(filename, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);

			/* Create buffer according to dimensions of image */
			Ogre::uint32 ImageHeight = HeightMap.getHeight(),
				ImageWidth = HeightMap.getWidth(),
				BufferSize = _Height * _Width;

			// DOING SOME HACKY ALLOCATION WITH NEW PLACEMENT
			_AllocateCellBuffer(BufferSize);
			for (Ogre::uint32 y = 0; y < HeightMap.getHeight(); y++) {
				for (Ogre::uint32 x = 0; x < HeightMap.getWidth(); x++) {

					/* Create enums according to bit fields */
					Ogre::RGBA ColourValue = HeightMap.getColourAt(x, y, 0).getAsRGBA();

					Cell::Zone Zone = static_cast<Cell::Zone>((ColourValue & ZONE_MASK) >> ZONE_SHIFT);
					Cell::Team Team = static_cast<Cell::Team>((ColourValue & TEAM_MASK) >> TEAM_SHIFT);
					Cell::Traverse Traverse = static_cast<Cell::Traverse>((ColourValue & TRAVERSE_MASK) >> TRAVERSE_SHIFT);

					/* Create new instance of the Cell */
					new (_Data + x + y * ImageWidth) Cell(x, y, Zone, Team, Traverse);
				}
			}

			_Width = ImageWidth;
			_Height = ImageHeight;
#endif
		}

		bool Data::HasData() const {
			return (_Data != NULL);
		}

		void Data::_AllocateCellBuffer(size_t bytes) {

			// WATCH OUT, THERE'S SOME HACKY STUFF HERE! INVOLVES PLACEMENT NEW
			// AND THE LIKE. NEEDS A SPECIAL DESTRUCTOR!

			/* Before we create it, need to destroy the existing buffer */
			_DestroyCellBuffer();

			_Buffer = new char[sizeof(Cell) * bytes];
			_Data = reinterpret_cast<Cell*>(_Buffer);
		}

		void Data::_DestroyCellBuffer() {
			/*
			* HACKY DESTRUCTION RIGHT HERE TO ENSURE THE DATA
			* HAS BEEN DESTROYED PROPERLY FROM PLACEMENT NEW
			*/
			if (_Data) {
				pos_type BufferSize = _Width * _Height;

				/* Call the constructor for each object stored */
				for (int i = 0; i < BufferSize; i++) {
					_Data[i].~Cell();
				}

				/* Delete the existing char buffer */
				delete[] _Buffer;

				/* Set values to NULL */
				_Data = NULL;
				_Buffer = NULL;
			}
		}
	}
}