/* Copyright 2023-2025 Keychron (https://www.keychron.com)
 * Copyright 2026 MeroFuruya (https://github.com/MeroFuruya)
 *
 * This file is based on the original Keychron Q1 Max keymap and contains
 * additional functionality and modifications by MeroFuruya.
 *
 * SPDX-License-Identifier: GPL-2.0-or-later
 */

#include QMK_KEYBOARD_H
#include "keychron_common.h"

// # Doc
// 
// ## Key Names
// 
// K  L  D F  S  M  _  C
// ^  ^  ^^^  ^  ^     ^
// |  |   |   |  |     > What does it do ("C" -> It goes to coding layer)
// |  |   |   |  > *M*ac or *W*indows
// |  |   |   > On what layer is it supposed to be used?
// |  |   > *DF* or *MO*
// |  > *L*ayer
// > *K*e
// 
// 

// Pre-Decl

bool is_mac_layer(uint8_t layer);

// ----- Util

void tap_code16_unmodded(uint16_t keycode) {
  uint8_t mods = get_mods();
  uint8_t weak_mods = get_weak_mods();

  clear_mods();
  clear_weak_mods();

  tap_code16(keycode);

  set_mods(mods);
  set_weak_mods(weak_mods);
}

// ----- Key Definition

#define DE_LPRN S(KC_8)  // (
#define DE_RPRN S(KC_9)  // )
#define DE_LBRC ALGR(KC_8)  // [
#define DE_RBRC ALGR(KC_9)  // ]
#define DE_LCBR ALGR(KC_7)  // {
#define DE_RCBR ALGR(KC_0)  // }

#define DE_MAC_LPRN S(KC_8)  // (
#define DE_MAC_RPRN S(KC_9)  // )
#define DE_MAC_LBRC A(KC_5)  // [
#define DE_MAC_RBRC A(KC_6)  // ]
#define DE_MAC_LCBR A(KC_8)  // {
#define DE_MAC_RCBR A(KC_9)  // }

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
  PAREN = SAFE_RANGE, // ()
  BRACKET, // []
  BRACE, // {}
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  uint8_t layer = get_highest_layer(layer_state | default_layer_state);
  bool mac_layer_active = is_mac_layer(layer);

  switch (keycode) {
    case PAREN: {
      if (!record->event.pressed) return true;
      
      if (!(get_mods() & MOD_MASK_SHIFT) && mac_layer_active) {
        tap_code16_unmodded(DE_MAC_LPRN);
        break;
      }
      
      if ((get_mods() & MOD_MASK_SHIFT) && mac_layer_active) {
        tap_code16_unmodded(DE_MAC_RPRN);
        break;
      }
      
      if (!(get_mods() & MOD_MASK_SHIFT) && !mac_layer_active) {
        tap_code16_unmodded(DE_LPRN);
        break;
      }
      
      if ((get_mods() & MOD_MASK_SHIFT) && !mac_layer_active) {
        tap_code16_unmodded(DE_RPRN);
        break;
      }
    } break;

    case BRACKET: {
      if (!record->event.pressed) return true;
      
      if (!(get_mods() & MOD_MASK_SHIFT) && mac_layer_active) {
        tap_code16_unmodded(DE_MAC_LBRC);
        break;
      }
      
      if ((get_mods() & MOD_MASK_SHIFT) && mac_layer_active) {
        tap_code16_unmodded(DE_MAC_RBRC);
        break;
      }
      
      if (!(get_mods() & MOD_MASK_SHIFT) && !mac_layer_active) {
        tap_code16_unmodded(DE_LBRC);
        break;
      }
      
      if ((get_mods() & MOD_MASK_SHIFT) && !mac_layer_active) {
        tap_code16_unmodded(DE_RBRC);
        break;
      }
    } break;
    
    case BRACE: {
      if (!record->event.pressed) return true;
      
      if (!(get_mods() & MOD_MASK_SHIFT) && mac_layer_active) {
        tap_code16_unmodded(DE_MAC_LCBR);
        break;
      }
      
      if ((get_mods() & MOD_MASK_SHIFT) && mac_layer_active) {
        tap_code16_unmodded(DE_MAC_RCBR);
        break;
      }
      
      if (!(get_mods() & MOD_MASK_SHIFT) && !mac_layer_active) {
        tap_code16_unmodded(DE_LCBR);
        break;
      }
      
      if ((get_mods() & MOD_MASK_SHIFT) && !mac_layer_active) {
        tap_code16_unmodded(DE_RCBR);
        break;
      }
    } break;
  }

  return true;
}

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
  LAYER_CODING_MAC_META,
  LAYER_CODING_MAC_FN,
  LAYER_CODING_WIN_BASE,
  LAYER_CODING_WIN_META,
  LAYER_CODING_WIN_FN,
};

bool is_mac_layer(uint8_t layer) {
  return layer == LAYER_STANDARD_MAC_BASE ||
    layer == LAYER_STANDARD_MAC_FN ||
    layer == LAYER_CODING_MAC_BASE ||
    layer == LAYER_CODING_MAC_FN;
}

// Layer-Keys

#define KLDFSM_C PDF(LAYER_CODING_MAC_BASE)
#define KLDFSW_C PDF(LAYER_CODING_WIN_BASE)

#define KLDFCM_S PDF(LAYER_STANDARD_MAC_BASE)
#define KLDFCW_S PDF(LAYER_STANDARD_WIN_BASE)

#define KLMOSM_F MO(LAYER_STANDARD_MAC_FN)
#define KLMOSW_F MO(LAYER_STANDARD_WIN_FN)

#define KLMOCM_F MO(LAYER_CODING_MAC_FN)
#define KLMOCW_F MO(LAYER_CODING_WIN_FN)

#define KLMOCM_M MO(LAYER_CODING_MAC_META)
#define KLMOCW_M MO(LAYER_CODING_WIN_META)

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  // Standard Layer
  [LAYER_STANDARD_MAC_BASE] = LAYOUT_iso_83(
    KC_ESC,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   KC_DEL,             KC_MUTE,
    KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,            KC_PGUP,
    KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,                      KC_PGDN,
    KC_ESC,   KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,  KC_NUHS,  KC_ENT,             KC_HOME,
    KC_LSFT,  KC_NUBS,  KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,            KC_RSFT,  KC_UP,
    KC_LCTL,  KC_LOPTN, KC_LCMMD,                               KC_SPC,                                 KC_RCMMD, KLMOSM_F, KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT
  ),

  [LAYER_STANDARD_MAC_FN] = LAYOUT_iso_83(
    _______,  KC_BRID,  KC_BRIU,  KC_MCTRL, KC_LNPAD, UG_VALD,  UG_VALU,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,  KC_VOLU,  KC_INS,             UG_TOGG,
    _______,  BT_HST1,  BT_HST2,  BT_HST3,  P2P4G,    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            XXXXXXX,
    UG_TOGG,  UG_NEXT,  UG_VALU,  UG_HUEU,  UG_SATU,  UG_SPDU,  _______,  _______,  _______,  _______,  _______,  _______,  _______,                      KLDFSM_C,
    KC_CAPS,  UG_PREV,  UG_VALD,  UG_HUED,  UG_SATD,  UG_SPDD,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            KC_END,
    _______,  _______,  _______,  _______,  _______,  _______,  BAT_LVL,  _______,  _______,  _______,  _______,  _______,            _______,  _______,
    _______,  _______,  _______,                                _______,                                _______,  _______,  _______,  _______,  _______,  _______
  ),

  [LAYER_STANDARD_WIN_BASE] = LAYOUT_iso_83(
    KC_ESC,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   KC_DEL,             KC_MUTE,
    KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,            KC_PGUP,
    KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_LBRC,  KC_RBRC,                      KC_PGDN,
    KC_ESC,   KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_SCLN,  KC_QUOT,  KC_NUHS,  KC_ENT,             KC_HOME,
    KC_LSFT,  KC_NUBS,  KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,            KC_RSFT,  KC_UP,
    KC_LCTL,  KC_LGUI,  KC_LALT,                                KC_SPC,                                 KC_RALT,  KLMOSW_F, KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT
  ),

  [LAYER_STANDARD_WIN_FN] = LAYOUT_iso_83(
    _______,  KC_BRID,  KC_BRIU,  KC_TASK,  KC_FILE,  UG_VALD,  UG_VALU,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,  KC_VOLU,  KC_INS,             UG_TOGG,
    _______,  BT_HST1,  BT_HST2,  BT_HST3,  P2P4G,    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            XXXXXXX,
    UG_TOGG,  UG_NEXT,  UG_VALU,  UG_HUEU,  UG_SATU,  UG_SPDU,  _______,  _______,  _______,  _______,  _______,  _______,  _______,                      KLDFSW_C,
    KC_CAPS,  UG_PREV,  UG_VALD,  UG_HUED,  UG_SATD,  UG_SPDD,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            KC_END,
    _______,  _______,  _______,  _______,  _______,  _______,  BAT_LVL,  _______,  _______,  _______,  _______,  _______,            _______,  _______,
    _______,  _______,  _______,                                _______,                                _______,  _______,  _______,  _______,  _______,  _______
  ),

  // Coding Layer
  [LAYER_CODING_MAC_BASE] = LAYOUT_iso_83(
    KC_ESC,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   KC_DEL,             KC_MUTE,
    KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,            KC_PGUP,
    KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     PAREN,    KC_RBRC,                      KC_PGDN,
    KLMOCM_M, KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     BRACKET,  BRACE,    KC_NUHS,  KC_ENT,             KC_HOME,
    KC_LSFT,  KC_NUBS,  KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,            KC_RSFT,  KC_UP,
    KC_LCTL,  KC_LOPTN, KC_LCMMD,                               KC_SPC,                                 KC_RCMMD, KLMOCM_F, KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT
  ),

  [LAYER_CODING_MAC_META] = LAYOUT_iso_83(
    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,                      _______,
    _______,  KC_LOPTN, KC_LSFT,  KC_BSPC,  _______,  _______,  KC_LEFT,  KC_DOWN,  KC_UP,    KC_RIGHT, _______,  _______,  _______,  _______,            _______,
    _______,  _______,  _______,  _______,  KC_LCTL,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,  _______,
    _______,  _______,  _______,                                _______,                                _______,  _______,  _______,  _______,  _______,  _______
  ),

  [LAYER_CODING_MAC_FN] = LAYOUT_iso_83(
    _______,  KC_BRID,  KC_BRIU,  KC_MCTRL, KC_LNPAD, UG_VALD,  UG_VALU,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,  KC_VOLU,  KC_INS,             UG_TOGG,
    _______,  BT_HST1,  BT_HST2,  BT_HST3,  P2P4G,    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            KLDFCM_S,
    UG_TOGG,  UG_NEXT,  UG_VALU,  UG_HUEU,  UG_SATU,  UG_SPDU,  _______,  _______,  _______,  _______,  _______,  KC_LBRC,  _______,                      XXXXXXX,
    KC_CAPS,  UG_PREV,  UG_VALD,  UG_HUED,  UG_SATD,  UG_SPDD,  _______,  _______,  _______,  _______,  KC_SCLN,  KC_QUOT,  _______,  _______,            KC_END,
    _______,  _______,  _______,  _______,  _______,  _______,  BAT_LVL,  _______,  _______,  _______,  _______,  _______,            _______,  _______,
    _______,  _______,  _______,                                _______,                                _______,  _______,  _______,  _______,  _______,  _______
  ),
  
  [LAYER_CODING_WIN_BASE] = LAYOUT_iso_83(
    KC_ESC,   KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,    KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,   KC_F11,   KC_F12,   KC_DEL,             KC_MUTE,
    KC_GRV,   KC_1,     KC_2,     KC_3,     KC_4,     KC_5,     KC_6,     KC_7,     KC_8,     KC_9,     KC_0,     KC_MINS,  KC_EQL,   KC_BSPC,            KC_PGUP,
    KC_TAB,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     PAREN,    KC_RBRC,                      KC_PGDN,
    KLMOCW_M, KC_A,     KC_S,     KC_D,     KC_F,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     BRACKET,  BRACE,    KC_NUHS,  KC_ENT,             KC_HOME,
    KC_LSFT,  KC_NUBS,  KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,     KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,            KC_RSFT,  KC_UP,
    KC_LCTL,  KC_LGUI,  KC_LALT,                                KC_SPC,                                 KC_RALT,  KLMOCW_F, KC_RCTL,  KC_LEFT,  KC_DOWN,  KC_RGHT
  ),

  [LAYER_CODING_WIN_META] = LAYOUT_iso_83(
    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,
    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,                      _______,
    _______,  KC_LALT,  KC_LSFT,  KC_BSPC,  _______,  _______,  KC_LEFT,  KC_DOWN,  KC_UP,    KC_RIGHT, _______,  _______,  _______,  _______,            _______,
    _______,  _______,  _______,  _______,  KC_LCTL,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            _______,  _______,
    _______,  _______,  _______,                                _______,                                _______,  _______,  _______,  _______,  _______,  _______
  ),

  [LAYER_CODING_WIN_FN] = LAYOUT_iso_83(
    _______,  KC_BRID,  KC_BRIU,  KC_TASK,  KC_FILE,  UG_VALD,  UG_VALU,  KC_MPRV,  KC_MPLY,  KC_MNXT,  KC_MUTE,  KC_VOLD,  KC_VOLU,  KC_INS,             UG_TOGG,
    _______,  BT_HST1,  BT_HST2,  BT_HST3,  P2P4G,    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            KLDFCW_S,
    UG_TOGG,  UG_NEXT,  UG_VALU,  UG_HUEU,  UG_SATU,  UG_SPDU,  _______,  _______,  _______,  _______,  _______,  _______,  _______,                      XXXXXXX,
    KC_CAPS,  UG_PREV,  UG_VALD,  UG_HUED,  UG_SATD,  UG_SPDD,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,            KC_END,
    _______,  _______,  _______,  _______,  _______,  _______,  BAT_LVL,  _______,  _______,  _______,  _______,  _______,            _______,  _______,
    _______,  _______,  _______,                                _______,                                _______,  _______,  _______,  _______,  _______,  _______
  ),
};
// clang-format on

// Encoder
#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
  // Standard Layer
  [LAYER_STANDARD_MAC_BASE] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)}, // Volume
  [LAYER_STANDARD_MAC_FN]   = {ENCODER_CCW_CW(UG_VALD, UG_VALU)}, // LED Brightness
  [LAYER_STANDARD_WIN_BASE] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)}, // Volume
  [LAYER_STANDARD_WIN_FN]   = {ENCODER_CCW_CW(UG_VALD, UG_VALU)}, // LED Brightness
  
  // Coding Layer
  [LAYER_CODING_MAC_BASE] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)}, // Volume
  [LAYER_CODING_MAC_META] = {ENCODER_CCW_CW(QK_MOUSE_WHEEL_UP, QK_MOUSE_WHEEL_DOWN)}, // Scroll
  [LAYER_CODING_MAC_FN]   = {ENCODER_CCW_CW(UG_VALD, UG_VALU)}, // LED Brightness
  [LAYER_CODING_WIN_BASE] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU)}, // Volume
  [LAYER_CODING_WIN_META] = {ENCODER_CCW_CW(QK_MOUSE_WHEEL_UP, QK_MOUSE_WHEEL_DOWN)}, // Scroll
  [LAYER_CODING_WIN_FN]   = {ENCODER_CCW_CW(UG_VALD, UG_VALU)}, // LED Brightness
};
#endif // ENCODER_MAP_ENABLE

