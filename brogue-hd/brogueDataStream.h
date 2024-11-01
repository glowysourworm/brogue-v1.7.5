#pragma once

#include "brogueProgram.h"
#include "brogueView.h"
#include "brogueViewProgram.h"
#include "brogueFlameMenu.h"
#include "brogueProgramBuilder.h"
#include "simpleDataStream.h"

namespace brogueHd::frontend::opengl
{
	template<typename T>
	concept isBrogueView = std::convertible_to<T, brogueView>;

	template<isBrogueView T>
	class brogueDataStream
	{
	public:

		brogueDataStream(resourceController* resourceController,
						 brogueGlyphMap* glyphMap,
						 openglDataStreamType dataStreamType,
						 openglBrogueCellOutputSelector noDisplaySelector,
						 bool aisFrameType)
		{
			programBuilder = new brogueProgramBuilder(resourceController, glyphMap);
			type = dataStreamType;
			outputSelector = noDisplaySelector;
			isFrameType = aisFrameType;
		}
		~brogueDataStream()
		{
			delete programBuilder;
		}

		virtual simpleDataStream* createStream(T* view, gridPredicate<brogueCellDisplay*> inclusionPredicate)
		{
			if (!isFrameType)
				return programBuilder->createSceneDataStream(view, type, outputSelector, inclusionPredicate);

			else
				return programBuilder->createFrameDataStream(view, type);
		}

		virtual simpleDataStream* createStream(T* view)
		{
			if (!isFrameType)
				return programBuilder->createSceneDataStream(view, type, outputSelector);

			else
				return programBuilder->createFrameDataStream(view, type);
		}

	protected:

		brogueProgramBuilder* programBuilder;

		openglDataStreamType type;
		openglBrogueCellOutputSelector outputSelector;
		bool isFrameType;
	};

	class heatSourceDataStream : public brogueDataStream<brogueFlameMenu>
	{
	public:

		heatSourceDataStream(resourceController* resourceController,
							 brogueGlyphMap* glyphMap,
							 openglDataStreamType dataStreamType,
							 openglBrogueCellOutputSelector noDisplaySelector,
							 bool isFrameType)
			: brogueDataStream<brogueFlameMenu>(resourceController, glyphMap, dataStreamType, noDisplaySelector, isFrameType)
		{
		}

		simpleDataStream* createStream(brogueFlameMenu* view)
		{
			return programBuilder->createSceneDataStream(view, type, outputSelector, [&view](short column, short row, brogueCellDisplay* cell)
			{
				return view->isTheText(column, row) || row == view->getBoundary().bottom();
			});
		}
	};

	class titleMaskDataStream : public brogueDataStream<brogueFlameMenu>
	{
	public:

		titleMaskDataStream(resourceController* resourceController,
							brogueGlyphMap* glyphMap,
							openglDataStreamType dataStreamType,
							openglBrogueCellOutputSelector noDisplaySelector,
							bool isFrameType)
			: brogueDataStream<brogueFlameMenu>(resourceController, glyphMap, dataStreamType, noDisplaySelector, isFrameType)
		{
		}

		simpleDataStream* createStream(brogueFlameMenu* view)
		{
			return programBuilder->createSceneDataStream(view, type, outputSelector, [&view] (short column, short row, brogueCellDisplay* cell)
			{
				return view->isTheText(column, row);
			});
		}
	};
}