#include <bbinput.h>
#include <bbgraphics.h>
#include <bbblitz3d.h>

#include "Inventory.h"
#include "Item.h"
#include "../GameMain.h"
#include "../Config/Options.h"
#include "../AssetMgmt/Assets.h"
#include "../AssetMgmt/Audio.h"
#include "../AssetMgmt/TextMgmt.h"
#include "../Player/Player.h"
#include "../Menus/Menu.h"
#include "../Map/MapSystem.h"

namespace CBN {

ItemCell::ItemCell() {
    hover = false;
    val = nullptr;
}

ItemCell::~ItemCell() {
    if (val != nullptr) {
        delete val;
    }
}

bool ItemCell::isHovering() {
    return hover;
}

void ItemCell::update(int x, int y) {
    if (bbMouseX() > x && bbMouseX() < x + ItemCell::SIZE) {
        if (bbMouseY() > y && bbMouseY() < y + ItemCell::SIZE) {
            hover = true;
            return;
        }
    }
    hover = false;
}

void ItemCell::draw(int x, int y, int cellSpacing) {
    if (hover) {
        bbColor(255, 0, 0);
        bbRect(x - 1, y - 1, SIZE + 2, SIZE + 2);
        bbColor(255, 255, 255);

        if (mainPlayer->selectedItem == nullptr && val != nullptr) {
            bbSetFont(uiAssets->font[0]);
            bbColor(0,0,0);
            bbText(x + SIZE / 2 + 1, y + SIZE + cellSpacing - 15 + 1, val->getInvName(), true);
            bbColor(255, 255, 255);
            bbText(x + SIZE / 2, y + SIZE + cellSpacing - 15, val->getInvName(), true);
        }
    }
    DrawFrame(x, y, SIZE, SIZE, (x % 64), (x % 64));

    if (val != nullptr) {
        // Render icon.
        val->generateInvImg();

        if (mainPlayer->selectedItem != val || mainPlayer->hoveredItemCell == this) {
            int offset = (int)(43 * MenuScale);
            bbDrawImage(val->invImg, x + SIZE / 2 - offset, y + SIZE / 2 - offset);
        }
    }
}

Inventory::Inventory(int size, int itemsPerRow) {
    items = new ItemCell[size];
    this->size = size;

    displayVertical = false;
    this->itemsPerRow = itemsPerRow;
    spacing = 35;
    xOffset = 0;
    yOffset = 0;
}

Inventory::~Inventory() {
    delete[] items;
}

int Inventory::getSize() const {
    return size;
}

void Inventory::addItem(Item* it) {
    bool spaceFound = false;
    for (int i = 0; i < size; i++) {
        if (items[i].val == nullptr) {
            setItem(it, i);
            spaceFound = true;
            break;
        }
    }
    if (!spaceFound) {
        throw ("No space in inventory.");
    }
}

void Inventory::setItem(Item* it, int slot) {
    if (slot >= size || slot < 0) {
        throw ("Invalid inventory slot. Slot \"" + String(slot) + "\" when only \"" + String(size) + "\" are available.");
    }
    items[slot].val = it;
    it->parentInv = this;
}

Item* Inventory::getItem(int slot) {
    if (slot >= size || slot < 0) {
        throw ("Invalid inventory slot. Slot \"" + String(slot) + "\" when only \"" + String(size) + "\" are available.");
    }
    if (items[slot].val == nullptr) {
        return nullptr;
    }
    return items[slot].val;
}

int Inventory::getIndex(Item* it) {
    for (int i = 0; i < size; i++) {
        if (items[i].val == it) {
            return i;
        }
    }

    return -1;
}

void Inventory::removeItem(Item* it) {
    it->parentInv = nullptr;
    for (int i = 0; i < size; i++) {
        if (items[i].val == it) {
            items[i].val = nullptr;
            return;
        }
    }

    throw ("Attempted to remove non-existant item \"" + it->getType() + " from inventory.\"");
}

bool Inventory::anyRoom() {
    for (int i = 0; i < size; i++) {
        if (items[i].val == nullptr) {
            return true;
        }
    }
    return false;
}

void Inventory::update() {
    int cellX = userOptions->screenWidth / 2 - (ItemCell::SIZE * itemsPerRow + spacing * (itemsPerRow - 1) / 2) + (int)(xOffset * MenuScale);
    int cellY = userOptions->screenHeight / 2 - ((ItemCell::SIZE + spacing) * (size / itemsPerRow) / 2 + spacing / 2) + (int)(yOffset * MenuScale);

    for (int i = 0; i < size; i++) {
        items[i].update(cellX, cellY);

        if (items[i].isHovering()) {
            mainPlayer->hoveredItemCell = &items[i];
            if (MouseHit1 && items[i].val != nullptr) {
                // Selecting an item.
                if (mainPlayer->selectedItem == nullptr) {
                    mainPlayer->selectedItem = items[i].val;
                }

                MouseHit1 = false;
                if (DoubleClick) {
                    // Using the item.
                    mainPlayer->useItem(mainPlayer->selectedItem);

                    mainPlayer->selectedItem = nullptr;
                    DoubleClick = false;
                }
            } else if (MouseUp1 && mainPlayer->selectedItem != nullptr) {
                // Item already selected and mouse release.

                // Hovering over empty slot. Move the item to the empty slot.
                if (items[i].val == nullptr) {
                    mainPlayer->moveItemToEmptySlot(mainPlayer->selectedItem, this, i);
                } else if (items[i].val != mainPlayer->selectedItem) {
                    // Hovering over another item. Attempt to combine the items.
                    items[i].val->combineWith(mainPlayer->selectedItem);
                }
                // Otherwise hovering over the item's own slot.

                mainPlayer->selectedItem = nullptr;
            }

            // If the mouse was hovering over this slot then don't bother iterating through the rest of the inventory.
            break;
        }

        // Move x and y coords to point to next item.
        if (!displayVertical) {
            cellX += ItemCell::SIZE + spacing;
            if (i % itemsPerRow == itemsPerRow-1) {
                cellY += ItemCell::SIZE + spacing;
                cellX = userOptions->screenWidth / 2 - (int)(xOffset * MenuScale);
            }
        } else {
            cellY += ItemCell::SIZE + spacing;
            if (i % itemsPerRow == itemsPerRow-1) {
                cellX += ItemCell::SIZE + spacing;
                cellY = userOptions->screenWidth / 2 - (int)(yOffset * MenuScale);
            }
        }
    }

        //Update any items that are used outside the inventory (firstaid for example).
    // TODO:
    // } else {
    //     if (player->selectedItem != nullptr) {
    //         if (MouseHit2) {
    //             //TODO: Move to de-equip function.
    //             bbEntityAlpha(player->overlays[OVERLAY_BLACK], 0.f);

    //             PlaySound_SM(sndMgmt->itemPick[(int)player->selectedItem->itemTemplate->sound]);
    //             player->selectedItem = nullptr;
    //         }
    //     }
    // }
}

void Inventory::draw() {
    int cellX = userOptions->screenWidth / 2 - (int)(xOffset * MenuScale);
    int cellY = userOptions->screenHeight / 2 - (int)(yOffset * MenuScale);

    for (int i = 0; i < size; i++) {
        items[i].draw(cellX, cellY, spacing);

        // Move x and y coords to point to next item.
        if (!displayVertical) {
            cellX += ItemCell::SIZE + spacing;
            if (i % itemsPerRow == itemsPerRow-1) {
                cellY += ItemCell::SIZE + spacing;
                cellX = userOptions->screenWidth / 2 - (int)(xOffset * MenuScale);
            }
        } else {
            cellY += ItemCell::SIZE + spacing;
            if (i % itemsPerRow == itemsPerRow-1) {
                cellX += ItemCell::SIZE + spacing;
                cellY = userOptions->screenWidth / 2 - (int)(yOffset * MenuScale);
            }
        }
    }

    // Draw the selected item under the cursor when it's not hovering over the item's original slot.
    if (mainPlayer->selectedItem != nullptr) {
        if (mainPlayer->hoveredItemCell == nullptr || mainPlayer->selectedItem != mainPlayer->hoveredItemCell->val) {
            bbDrawImage(mainPlayer->selectedItem->invImg, bbMouseX() - bbImageWidth(mainPlayer->selectedItem->invImg) / 2, bbMouseY() - bbImageHeight(mainPlayer->selectedItem->invImg) / 2);
        }
    }
}

}