/* Copyright 2023 ~ 2025 @ Keychron (https://www.keychron.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include QMK_KEYBOARD_H
#include "keychron_common.h"

// ----- Setup
static uint32_t startup_time;

void keyboard_post_init_user(void) {
  startup_time = timer_read32();
}

void eeconfig_init_user(void) {
  rgb_matrix_enable();
  rgb_matrix_mode(RGB_MATRIX_SOLID_COLOR);
  rgb_matrix_sethsv(234, 128, 100); // Low Pink
}

// ----- Key Codes

enum custom_keycodes {
  COD_AE = SAFE_RANGE,
  COD_OE,
  COD_UE,
};

// ----- Combos

enum combos {
  COMBO_BOOTLOADER,
};

const uint16_t PROGMEM combo_keys_bootloader[] = {KC_J, KC_K, COMBO_END};

combo_t key_combos[] = {
  [COMBO_BOOTLOADER] = COMBO(combo_keys_bootloader, QK_BOOT),
};


bool combo_should_trigger(uint16_t combo_index, combo_t *combo, uint16_t keycode, keyrecord_t *record) {
  switch (combo_index) {
    case COMBO_BOOTLOADER:
      return timer_elapsed32(startup_time) < 5000;
  }

  return true;
}

// ----- Layers

enum layers {
  // Standard Layer
  LAYER_STANDARD_MAC_BASE,
  LAYER_STANDARD_MAC_FN,
  LAYER_STANDARD_WIN_BASE,
  LAYER_STANDARD_WIN_FN,
  
  // Coding Layer
  LAYER_CODING_MAC_BASE,
  LAYER_CODING_MAC_FN,
  LAYER_CODING_WIN_BASE,
  LAYER_CODING_WIN_FN,
  
};

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  // Standard Layer
  [LAYER_STANDARD_MAC_BASE] = LAYOUT_iso_83(
    KC_ESC,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,                      KC_F12,   KC_DEL,             KC_MUTE,
    KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,                     KC_EQL,   KC_BSPC,            KC_PGUP,
    KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,                     KC_RBRC,                      KC_PGDN,
    KC_ESC,   KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,                     KC_NUHS,  KC_ENT,             KC_HOME,
    KC_LSFT,  KC_NUBS,  KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,                               KC_RSFT,  KC_UP,
    KC_LCTL,  KC_LOPTN, KC_LCMMD,                               KC_SPC,                                 KC_RCMMD, MO(LAYER_STANDARD_MAC_FN),   KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT
  ),

  [LAYER_STANDARD_MAC_FN] = LAYOUT_iso_83(
    _______,  KC_BRID,  KC_BRIU,  KC_MCTRL, KC_LNPAD, UG_VALD,  UG_VALU,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,                     KC_VOLU,  _______,            UG_TOGG,
    _______,  BT_HST1,  BT_HST2,  BT_HST3,  P2P4G,    _______,  _______,  _______,  _______,  _______,  _______,  _______,                     _______,  _______,            DF(LAYER_STANDARD_MAC_BASE),
    UG_TOGG,  UG_NEXT,  UG_VALU,  UG_HUEU,  UG_SATU,  UG_SPDU,  _______,  _______,  _______,  _______,  _______,  _______,                     _______,                      DF(LAYER_CODING_MAC_BASE),
    KC_CAPS,  UG_PREV,  UG_VALD,  UG_HUED,  UG_SATD,  UG_SPDD,  _______,  _______,  _______,  _______,  _______,  _______,                     _______,  _______,            KC_END,
    _______,  _______,  _______,  _______,  _______,  _______,  BAT_LVL,  _______,  _______,  _______,  _______,  _______,                               _______,  _______,
    _______,  _______,  _______,                                _______,                                _______,  _______,                     _______,  _______,  _______,  _______
  ),

  [LAYER_STANDARD_WIN_BASE] = LAYOUT_iso_83(
    KC_ESC,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,                      KC_F12,   KC_DEL,             KC_MUTE,
    KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,                     KC_EQL,   KC_BSPC,            KC_PGUP,
    KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,                     KC_RBRC,                      KC_PGDN,
    KC_ESC,   KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,                     KC_NUHS,  KC_ENT,             KC_HOME,
    KC_LSFT,  KC_NUBS,  KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,                               KC_RSFT,  KC_UP,
    KC_LCTL,  KC_LGUI,  KC_LALT,                                KC_SPC,                                 KC_RALT,  MO(LAYER_STANDARD_WIN_FN),   KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT
  ),

  [LAYER_STANDARD_WIN_FN] = LAYOUT_iso_83(
    _______,  KC_BRID,  KC_BRIU,  KC_TASK,  KC_FILE,  UG_VALD,  UG_VALU,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,                     KC_VOLU,  _______,            UG_TOGG,
    _______,  BT_HST1,  BT_HST2,  BT_HST3,  P2P4G,    _______,  _______,  _______,  _______,  _______,  _______,  _______,                     _______,  _______,            DF(LAYER_STANDARD_WIN_BASE),
    UG_TOGG,  UG_NEXT,  UG_VALU,  UG_HUEU,  UG_SATU,  UG_SPDU,  _______,  _______,  _______,  _______,  _______,  _______,                     _______,                      DF(LAYER_CODING_WIN_BASE),
    KC_CAPS,  UG_PREV,  UG_VALD,  UG_HUED,  UG_SATD,  UG_SPDD,  _______,  _______,  _______,  _______,  _______,  _______,                     _______,  _______,            KC_END,
    _______,  _______,  _______,  _______,  _______,  _______,  BAT_LVL,  _______,  _______,  _______,  _______,  _______,                               _______,  _______,
    _______,  _______,  _______,                                _______,                                _______,  _______,                     _______,  _______,  _______,  _______
  ),

  // Coding Layer
  [LAYER_CODING_MAC_BASE] = LAYOUT_iso_83(
    KC_ESC,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,                    KC_F12,   KC_DEL,             KC_MUTE,
    KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,                   KC_EQL,   KC_BSPC,            KC_PGUP,
    KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     LSFT(KC_8),                KC_RBRC,                      KC_PGDN,
    KC_ESC,   KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     LOPT(KC_5),  LOPT(KC_8),             KC_NUHS,  KC_ENT,             KC_HOME,
    KC_LSFT,  KC_NUBS,  KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,                             KC_RSFT,  KC_UP,
    KC_LCTL,  KC_LOPTN, KC_LCMMD,                               KC_SPC,                                 KC_RCMMD, MO(LAYER_CODING_MAC_FN),   KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT
  ),

  [LAYER_CODING_MAC_FN] = LAYOUT_iso_83(
    _______,  KC_BRID,  KC_BRIU,  KC_MCTRL, KC_LNPAD, UG_VALD,  UG_VALU,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,                   KC_VOLU,  _______,            UG_TOGG,
    _______,  BT_HST1,  BT_HST2,  BT_HST3,  P2P4G,    _______,  _______,  _______,  _______,  _______,  _______,  _______,                   _______,  _______,            DF(LAYER_STANDARD_MAC_BASE),
    UG_TOGG,  UG_NEXT,  UG_VALU,  UG_HUEU,  UG_SATU,  UG_SPDU,  _______,  _______,  _______,  _______,  _______,  KC_LBRC,                   _______,                      DF(LAYER_CODING_MAC_BASE),
    KC_CAPS,  UG_PREV,  UG_VALD,  UG_HUED,  UG_SATD,  UG_SPDD,  _______,  _______,  _______,  _______,  KC_SCLN,  KC_QUOT,                   _______,  _______,            KC_END,
    _______,  _______,  _______,  _______,  _______,  _______,  BAT_LVL,  _______,  _______,  _______,  _______,  _______,                             _______,  _______,
    _______,  _______,  _______,                                _______,                                _______,  _______,                   _______,  _______,  _______,  _______
  ),

  [LAYER_CODING_WIN_BASE] = LAYOUT_iso_83(
    KC_ESC,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,                    KC_F12,   KC_DEL,             KC_MUTE,
    KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,                   KC_EQL,   KC_BSPC,            KC_PGUP,
    KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,                   KC_RBRC,                      KC_PGDN,
    KC_ESC,   KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,                   KC_NUHS,  KC_ENT,             KC_HOME,
    KC_LSFT,  KC_NUBS,  KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,                             KC_RSFT,  KC_UP,
    KC_LCTL,  KC_LGUI,  KC_LALT,                                KC_SPC,                                 KC_RALT,  MO(LAYER_CODING_WIN_FN),   KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT
  ),

  [LAYER_CODING_WIN_FN] = LAYOUT_iso_83(
    _______,  KC_BRID,  KC_BRIU,  KC_TASK,  KC_FILE,  UG_VALD,  UG_VALU,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,                   KC_VOLU,  _______,            UG_TOGG,
    _______,  BT_HST1,  BT_HST2,  BT_HST3,  P2P4G,    _______,  _______,  _______,  _______,  _______,  _______,  _______,                   _______,  _______,            DF(LAYER_STANDARD_WIN_BASE),
    UG_TOGG,  UG_NEXT,  UG_VALU,  UG_HUEU,  UG_SATU,  UG_SPDU,  _______,  _______,  _______,  _______,  _______,  _______,                   _______,                      DF(LAYER_CODING_WIN_BASE),
    KC_CAPS,  UG_PREV,  UG_VALD,  UG_HUED,  UG_SATD,  UG_SPDD,  _______,  _______,  _______,  _______,  _______,  _______,                   _______,  _______,            KC_END,
    _______,  _______,  _______,  _______,  _______,  _______,  BAT_LVL,  _______,  _______,  _______,  _______,  _______,                             _______,  _______,
    _______,  _______,  _______,                                _______,                                _______,  _______,                   _______,  _______,  _______,  _______
  ),
};
// clang-format on

// Encoder
#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
  // Standard Layer
  [LAYER_STANDARD_MAC_BASE] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
  [LAYER_STANDARD_MAC_FN]   = {ENCODER_CCW_CW(UG_VALD, UG_VALU)},
  [LAYER_STANDARD_WIN_BASE] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
  [LAYER_STANDARD_WIN_FN]   = {ENCODER_CCW_CW(UG_VALD, UG_VALU)},
  
  // Coding Layer
  [LAYER_CODING_MAC_BASE] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
  [LAYER_CODING_MAC_FN]   = {ENCODER_CCW_CW(UG_VALD, UG_VALU)},
  [LAYER_CODING_WIN_BASE] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)},
  [LAYER_CODING_WIN_FN]   = {ENCODER_CCW_CW(UG_VALD, UG_VALU)},
};
#endif // ENCODER_MAP_ENABLE

