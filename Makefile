QMK_BIN := qmk

.PHONY: all
all: compile 

.PHONY: compile
compile:
	$(QMK_BIN) compile -kb keychron/q1_max/iso_encoder -km merofuruya

.PHONY: compile
flash:
	$(QMK_BIN) flash -kb keychron/q1_max/iso_encoder -km merofuruya


