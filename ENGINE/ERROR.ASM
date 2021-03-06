; ============================================================================================
; LUMINARY - a game engine and framework for the SEGA Mega Drive
; ============================================================================================
; Matt Phillips - Big Evil Corporation Ltd - 2nd August 2019
; ============================================================================================
; ERROR.ASM - Asserts and error message handling
; ============================================================================================

ERR_TEXT_POS_X  equ 0x04
ERR_TEXT_POS_Y  equ 0x02

ERR_ResetForError:
    ; ======================================
    ; Resets the VDP and loads debug font
    ; and palette, in preparation to draw
    ; an error message
    ; ======================================
    ; No params
    ; ======================================

    ; Initialise TMSS
    bsr    VDP_WriteTMSS

    ; Load default VDP regs
    lea    default_vdp_regs, a0
    bsr    VDP_LoadRegisters

    ; Reset scroll
    VDP_SET_VRAM_WRITE VRAM_ADDR_HSCROLL
    move.w #0x0000, PORT_VDP_DATA
    VDP_SET_VSRAM_WRITE 0x0000
    move.w #0x0000, PORT_VDP_DATA

    ; Load debug font
    lea    debug_font, a0
    move.w #0x0, d0
    move.w #SIZEOF_debug_font/SIZE_TILE_B, d1
    bsr    VDP_LoadTiles
    move.l #0x0, RAM_DBG_FONT_VRAM

    ; Load debug palette
    lea    error_palette, a0
    moveq  #0x0, d0
    bsr    VDP_LoadPalette

    rts

ERR_DrawMessage:
    ; ======================================
    ; Draws an error message.
    ; Resets VDP regs, loads debug font
    ; at VRAM 0x0000.
    ; ======================================
    ; a0   Error string
    ; ======================================

    move.w #ERR_TEXT_POS_X, d0
    move.w #ERR_TEXT_POS_Y, d1
    bsr    DBG_DrawString

    rts