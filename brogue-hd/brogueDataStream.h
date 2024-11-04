#pragma once

#include "brogueCellDisplay.h"
#include "brogueFlameMenu.h"
#include "brogueGlyphMap.h"
#include "brogueProgramBuilder.h"
#include "brogueUIConstants.h"
#include "brogueView.h"
#include "gridDefinitions.h"
#include "resourceController.h"
#include "simpleDataStream.h"
#include "simpleException.h"

using namespace brogueHd::simple;
using namespace brogueHd::component;
using namespace brogueHd::backend::model::layout;

namespace brogueHd::frontend::opengl
{
	class brogueDataStream
	{
	public:
		brogueDataStream() {}
		brogueDataStream(resourceController* resourceController,
			brogueGlyphMap* glyphMap,
			brogueOpenglDataStream adataStreamName,
			openglDataStreamType dataStreamType,
			openglBrogueCellOutputSelector noDisplaySelector,
			bool aisFrameType)
		{
			programBuilder = new brogueProgramBuilder(resourceController, glyphMap);
			type = dataStreamType;
			outputSelector = noDisplaySelector;
			isFrameType = aisFrameType;
			dataStreamName = adataStreamName;
		}
		~brogueDataStream()
		{
			delete programBuilder;
		}

		simpleDataStream* createStream(brogueView* view, gridPredicate<brogueCellDisplay*> inclusionPredicate)
		{
			switch (dataStreamName)
			{
				case brogueOpenglDataStream::BrogueView:
				{
					if (!isFrameType)
						return programBuilder->createSceneDataStream(view, type, outputSelector, inclusionPredicate);

					else
						return programBuilder->createFrameDataStream(view, type);
				}
				case brogueOpenglDataStream::HeatSourceStream:
				{
					return createHeatSourceStream((brogueFlameMenu*)view);
				}
				case brogueOpenglDataStream::TitleMaskStream:
				{
					return createTitleMaskStream((brogueFlameMenu*)view);
				}
				default:
					simpleException::show("Unhandled brogueOpenglDataStream type:  brogueDataStream.h");
			}
		}

		simpleDataStream* createStream(brogueView* view)
		{
			switch (dataStreamName)
			{
				case brogueOpenglDataStream::BrogueView:
				{
					if (!isFrameType)
						return programBuilder->createSceneDataStream(view, type, outputSelector);

					else
						return programBuilder->createFrameDataStream(view, type);
				}
				case brogueOpenglDataStream::HeatSourceStream:
				{
					return createHeatSourceStream((brogueFlameMenu*)view);
				}
				case brogueOpenglDataStream::TitleMaskStream:
				{
					return createTitleMaskStream((brogueFlameMenu*)view);
				}
				default:
					simpleException::show("Unhandled brogueOpenglDataStream type:  brogueDataStream.h");
			}
		}

		simpleDataStream* createHeatSourceStream(brogueFlameMenu* view)
		{
			return programBuilder->createSceneDataStream(view, type, outputSelector, [&view] (short column, short row, brogueCellDisplay* cell)
			{
				return view->isTheText(column, row) || row == view->getBoundary().bottom();
			});
		}

		simpleDataStream* createTitleMaskStream(brogueFlameMenu* view)
		{
			return programBuilder->createSceneDataStream(view, type, outputSelector, [&view] (short column, short row, brogueCellDisplay* cell)
			{
				return view->isTheText(column, row);
			});
		}

	protected:

		brogueProgramBuilder* programBuilder;

		brogueOpenglDataStream dataStreamName;
		openglDataStreamType type;
		openglBrogueCellOutputSelector outputSelector;
		bool isFrameType;
	};
}