.section .rodata
    .global settings_root
	.type   settings_root, @object
	.align  4
    .global orbis_toolbox
	.type   orbis_toolbox, @object
	.align  4
    .global external_hdd
	.type   external_hdd, @object
	.align  4

settings_root:
    .incbin "settings_root.xml"
settings_root_End:
    .global settings_root_Size
    .type   settings_root_Size, @object
    .align  4
settings_root_Size:
    .int    settings_root_End - settings_root

orbis_toolbox:
    .incbin "orbis_toolbox.xml"
orbis_toolbox_End:
    .global orbis_toolbox_Size
    .type   orbis_toolbox_Size, @object
    .align  4
orbis_toolbox_Size:
    .int    orbis_toolbox_End - orbis_toolbox

external_hdd:
    .incbin "external_hdd.xml"
external_hdd_End:
    .global external_hdd_Size
    .type   external_hdd_Size, @object
    .align  4
external_hdd_Size:
    .int    external_hdd_End - external_hdd