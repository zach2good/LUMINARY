; ============================================================================================
;   AUTOGENERATED WITH BEEHIVE - DO NOT EDIT MANUALLY
;============================================================================================
;   http://www.bigevilcorporation.co.uk
; ============================================================================================
;   Beehive and LUMINARY Engine (c) Matt Phillips 2019
; ============================================================================================


actor_test2_spritesheet_test2:
	dc.l actor_test2_sheet_test2_frame_0_header	; SpriteSheet_FirstFrame
	dc.w 0x04	; SpriteSheet_VRAMSizeTiles

actor_test2_sheet_test2_frame_0_header:
	dc.w 0x0004	; SpriteFrame_SizeTiles
	dc.w actor_test2_sheet_test2_frame_0_size_subsprites	; SpriteFrame_SizeSubsprites
	dc.l actor_test2_sheet_test2_frame_0	; SpriteFrame_TileData
	dc.l actor_test2_sheet_test2_frame_0_subsprite_dimensions_bits	; SpriteFrame_LayoutTable
	dc.l actor_test2_sheet_test2_frame_0_subsprite_pos_offsets	; SpriteFrame_PosOffsetTable

actor_test2_sheet_test2_frame_0_size_b		equ 0x0080	; Size in bytes
actor_test2_sheet_test2_frame_0_size_t		equ 0x0004	; Size in tiles
actor_test2_sheet_test2_frame_0_size_subsprites		equ 0x0001	; Size in subsprites

; Subsprite offsets from 0,0 (in pixels) - unflipped (ww) and flipped X (ww)
actor_test2_sheet_test2_frame_0_subsprite_pos_offsets:
	dc.l 0x00000000, 0x00000000
	Even

; Subsprite dimension bits (for sprite descs)
actor_test2_sheet_test2_frame_0_subsprite_dimensions_bits:
	dc.b 0x5
	even


spritesheets_test2_tiledata:

actor_test2_sheet_test2_tiledata:

actor_test2_sheet_test2_frame_0:
	dc.l	0x00000BBB
	dc.l	0x0000AABB
	dc.l	0x000AAABB
	dc.l	0x0A0AA9BB
	dc.l	0x0AAA922B
	dc.l	0xBAA9221B
	dc.l	0x0BA9221B
	dc.l	0xBBBA222B

	dc.l	0xBBBA99AA
	dc.l	0x00BABBAA
	dc.l	0xBBBBBBBB
	dc.l	0x0BBABBBB
	dc.l	0x00BBAABB
	dc.l	0x00B0AABB
	dc.l	0x000BBABB
	dc.l	0x00000B0B

	dc.l	0xB0B00000
	dc.l	0xAABBB000
	dc.l	0xBBBB0B00
	dc.l	0xBB9BABB0
	dc.l	0xB229AAB0
	dc.l	0xB122ABBB
	dc.l	0xA1229AB0
	dc.l	0xB2229B00

	dc.l	0xAB99BBAB
	dc.l	0xBBBBBBBB
	dc.l	0xBBBBBBBB
	dc.l	0xBBBBB0B0
	dc.l	0xBABBBBB0
	dc.l	0xBBABBB00
	dc.l	0x00BBA000
	dc.l	0xB0A00000




