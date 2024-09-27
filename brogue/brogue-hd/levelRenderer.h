#pragma once

#include "brogueLevel.h"
#include "brogueMessageQueue.h"
#include "openglRenderer.h"
#include "brogueView.h"

using namespace brogueHd::backend::model;
using namespace brogueHd::backend::generator;
using namespace brogueHd::frontend::opengl;

namespace brogueHd::frontend::renderer
{
	class levelRenderer : public openglRenderer
	{
	public:

		levelRenderer(brogueLevel* level, brogueMessageQueue* messageQueue, randomGenerator* randomGenerator);
		~levelRenderer();

		/// <summary>
		/// Primary method to update all views for level
		/// </summary>
		void update() const;

		brogueView* getLevelView() const;
		brogueView* getRememberedView() const;
		brogueView* getSidebarView() const;
		brogueView* getFooterMenuView() const;
		brogueView* getInventoryView() const;
		brogueView* getMessageView() const;
		brogueView* getFlavorMessageView() const;

		//void plotCharWithColor(uchar inputChar, short xLoc, short yLoc, const color* cellForeColor, const color* cellBackColor);

		//void hiliteGrid(color* hiliteColor, short hiliteStrength);

		//void bakeColor(color* theColor);

	private:

		brogueLevel* _level;
		brogueMessageQueue* _messageQueue;
		randomGenerator* _randomGenerator;

		// Display Views:  The likely rendering situation will be to use transparency to piece the view 
		//				   together into layers. This will depend on how Brogue originally functioned.
		//				
		brogueView* _levelView;					// Map of visible tiles
		brogueView* _rememberedView;			// Map of remembered tiles

		brogueView* _sidebarView;				// Sidebar of enemy information
		brogueView* _footerMenuView;			// Menu at the bottom of the level screen
		brogueView* _inventoryView;				// Inventory layover to the top right

		brogueView* _messageView;				// Message archive view - full sized
		brogueView* _flavorMessageView;			// Flavor text bar
	};
}

