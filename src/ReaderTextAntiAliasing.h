#pragma once

#include <BoardConfig.h>
#include <I18n.h>

#include <iterator>

#include "CrossPointSettings.h"

// Soft differs from Standard only where the panel driver draws the darkest
// overlay gray as black unless asked for full tone (EEGO A4). Other boards
// keep the on/off toggle.
inline bool textAntiAliasingHasSoft() { return BoardConfig::isEegoA4(); }

// Labels follow the persisted TEXT_ANTI_ALIASING order.
inline constexpr StrId TEXT_AA_IDS[] = {StrId::STR_STATE_OFF, StrId::STR_TEXT_AA_STANDARD, StrId::STR_TEXT_AA_SOFT};
static_assert(std::size(TEXT_AA_IDS) == CrossPointSettings::TEXT_ANTI_ALIASING_COUNT);
