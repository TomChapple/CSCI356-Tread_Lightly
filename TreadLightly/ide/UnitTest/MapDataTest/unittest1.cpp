#include "stdafx.h"
#include "CppUnitTest.h"

#include <Windows.h>
#include <OgreResourceGroupManager.h>
#include <OgreRoot.h>

#include "map/MapData.h"
#include "app/TreadLightlyApp.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace TreadLightly::MapUtilities;

namespace TreadLight {
	namespace MapDataTest
	{		
		TEST_CLASS(MapDataTest)
		{
		public:
		
			TEST_METHOD(TestCell)
			{
				/* Test copy constructors */
				Cell FirstCell(5, 4, Cell::ZONE_MAZE, Cell::TEAM_THREE, Cell::TRAVERSE_FREE);
				Cell SecondCell(FirstCell);
				Assert::AreEqual(FirstCell.GetX(), SecondCell.GetX(), L"Copied cell X does not match");
				Assert::AreEqual(FirstCell.GetY(), SecondCell.GetY(), L"Copied cell Y does not match");
				Assert::AreEqual((int)FirstCell.GetZone(), (int)SecondCell.GetZone(), L"Copied cell Zone does not match");
				Assert::AreEqual((int)FirstCell.GetTeam(), (int)SecondCell.GetTeam(), L"Copied cell Team does not match");
				Assert::AreEqual((int)FirstCell.GetTraverseType(), (int)SecondCell.GetTraverseType(), L"Copied cell Traverse does not match");

				/* Tets equals operator */
				FirstCell = Cell(1, 2, Cell::ZONE_BATTLEFIELD, Cell::TEAM_TWO, Cell::TRAVERSE_GROUND_BLOCKED);
				Assert::AreEqual(1, FirstCell.GetX(), L"X not re-initialised");
				Assert::AreEqual(2, FirstCell.GetY(), L"Y not re-initialised");
				Assert::AreEqual((int)Cell::ZONE_BATTLEFIELD, (int)FirstCell.GetZone(), L"Zone not re-initialised");
				Assert::AreEqual((int)Cell::TEAM_TWO, (int)FirstCell.GetTeam(), L"Team not re-initialised");
				Assert::AreEqual((int)Cell::TRAVERSE_GROUND_BLOCKED, (int)FirstCell.GetTraverseType(), L"Traverse not re-initialised");
			}

			TEST_METHOD(TestIterator)
			{
				/* Setup Resource Groups */
				TreadLightly::TreadLightlyApp app;

				/* Create Data to run on */
				Data TestData("testmap.bmp");

				/* Create NULL Iterator */
				Data::iterator TestIt;
				try {
					*TestIt;
					Assert::Fail(L"Iterator should not be de-referencable");
				}
				catch (std::out_of_range e) {

				}

				try {
					TestIt = TestData->begin();
				}
				catch (std::runtime_error e) {
					Assert::Fail(L"Error caught when getting begin iterator");
				}
			}

		};
	}
}