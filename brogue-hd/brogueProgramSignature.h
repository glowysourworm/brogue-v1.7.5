#pragma once

#include "brogueUIConstants.h"
#include "simple.h"

namespace brogueHd::frontend::opengl
{
	struct brogueProgramSignature : hashable
	{
		brogueProgramSignature()
		{
			view = brogueUIView::Unnamed;
			program = brogueUIProgram::ContainerControlledProgram;
			container = brogueUIContainer::TitleContainer;
			purpose = brogueProgramPurpose::None;
		}
		brogueProgramSignature(brogueUIView aview, brogueUIProgram aprogram, brogueUIContainer acontainer, brogueProgramPurpose apurpose)
		{
			view = aview;
			program = aprogram;
			container = acontainer;
			purpose = apurpose;
		}
		brogueProgramSignature(const brogueProgramSignature& copy)
		{
			view = copy.view;
			program = copy.program;
			container = copy.container;
			purpose = copy.purpose;
		}
		void operator=(const brogueProgramSignature& copy)
		{
			view = copy.view;
			program = copy.program;
			container = copy.container;
			purpose = copy.purpose;
		}
		bool operator==(const brogueProgramSignature& other)
		{
			return compare(other);
		}
		bool operator!=(const brogueProgramSignature& other)
		{
			return !compare(other);
		}

		brogueUIView view;
		brogueUIProgram program;
		brogueUIContainer container;
		brogueProgramPurpose purpose;

		size_t getHash() const override
		{
			return hashGenerator::generateHash(view, program, container, purpose);
		}

	private:

		bool compare(const brogueProgramSignature& other)
		{
			return view == other.view &&
				program == other.program &&
				container == other.container &&
				purpose == other.purpose;
		}
	};
}