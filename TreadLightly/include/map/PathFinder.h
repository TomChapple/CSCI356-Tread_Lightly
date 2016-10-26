/*
* TREAD LIGHTLY
* Authors: Tom Chapple (26/10)
* File: PathFinder.h
* Last modified on: 26/10/16
* Purpose: Describes the class used to represent
* pathfinding through a network of node. In this
* case, it uses MapData.
*/

#pragma once

#include "MapData.h"

namespace TreadLightly {

	namespace MapUtilities {

		class PathFinder {

		public:
			/* ~~~ Constructors / Destructors ~~~ */
			PathFinder(const Data& mapData);
			~PathFinder();

			/* ~~~ Interface Methods ~~~ */

			template <typename Container>
			FindPath(const Cell& from, const Cell& to, Container<Cell>& insert);

			template <typename Container>
			FindPath(const std::pair<pos_type, pos_type>& from,
				const std::pair<pos_type, pos_type>& to,
				Container<Cell>& insert);

			template <typename Container>
			FindPath(pos_type fromX, pos_type fromY,
				pos_type toX, pos_type toY,
				Container<Cell>& insert);

			void SetData(const Data& mapData);
			const Data *GetData() const;

		protected:

			/* ~~~ Members ~~~ */
			const Data *_Data;


		};
	}
}