/*
* TREAD LIGHTLY
* Authors: Tom Chapple (23/10/16)
* File: MapData.h
* Last Edited on: 23/10/16
* Purpose: This file describes the abstract, grid-based map
* data loaded in from a height map. As such, it needs an
* intermediary to convert between this and the game world.
*/

#pragma once

#include <string>

namespace TreadLightly {

	namespace MapUtilities {

		/** The type to be used to represent the grid dimensions
		* and position in the Map.
		*/
		typedef int pos_type;

		/** An immutable object representing a single
		* grid location in the Map.
		*/
		class Cell {

		public:

			/* ~~~ Enumerations ~~~ */

			enum Zone {
				ZONE_SPAWN = 0,
				ZONE_MAZE,
				ZONE_BATTLEFIELD
			};

			enum Team {
				TEAM_NONE = 0,
				TEAM_ONE,
				TEAM_TWO,
				TEAM_THREE,
				TEAM_FOUR
			};

			enum Traverse {
				TRAVERSE_FREE = 0,
				TRAVERSE_GROUND_BLOCKED
			};

			/* ~~~ Constructors / Destructors ~~~ */

			Cell(pos_type x, pos_type y, Zone zone, Team team, Traverse trav);
			Cell(const Cell& x);
			~Cell();

			/* ~~~ Accessors ~~~ */

			pos_type GetX() const;
			pos_type GetY() const;
			std::pair<pos_type, pos_type> GetPoint() const;

			Zone GetZone() const;
			Team GetTeam() const;
			Traverse GetTraverseType() const;

			bool IsTraversable() const;

			/* ~~~ Operators ~~~ */

			Cell& operator=(const Cell& right);

		protected:
			/* ~~~ Members ~~~ */

			pos_type _X, _Y;
			Zone _Zone;
			Team _Team;
			Traverse _Traverse;
		};

		/** Contains the abstract data represent a grid map
		*/
		class Data {

		protected:
			typedef int pos_type;

		public:

			/* ~~~ Types / Typedefs ~~~ */

			class iterator {
				friend class Data;

			public:
				/* ~~~ Constructors / Destructors ~~~ */
				iterator();
				iterator(const iterator& x);
				~iterator();

				/* ~~~ Operators ~~~ */

				iterator& operator=(const iterator& right);

				Cell operator*() const;
				Cell *operator->() const;

				iterator& operator++();
				iterator operator++(int);

				iterator& operator--();
				iterator operator--(int);

				bool operator==(const iterator& right) const;
				bool operator!=(const iterator& right) const;

			protected:

				/* ~~~ Members ~~~ */
				Cell *_Ptr;
				pos_type _Offset, _MaxOffset;

				/* ~~~ Internal Functions ~~~*/
				inline bool _ValidateIterator() const;
			};

			typedef iterator const_iterator;

			/* ~~~ Constructors / Destructors ~~~ */

			Data();
			Data(const std::string& path);
			~Data();

			/* ~~~ Methods ~~~ */

			pos_type GetWidth() const;
			pos_type GetHeight() const;

			const_iterator begin() const;
			const_iterator end() const;

			Cell GetCell(pos_type x, pos_type y) const;

			template <typename Container>
			void GetAdjacentCells(axis_t x, axis_t y, Container<Cell>& insert) const;

			float Heuristic(const Cell& from, const Cell& to) const;

			void LoadFromFile(const std::string& filename);

			bool HasData() const;

		protected:

			/* ~~~ Members ~~~ */

			bool _HasData;
			Cell *_Data;

			pos_type _Width, _Height;
		};
	}
}