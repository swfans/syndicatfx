TEXTDAT = $(PYTHON) $(top_srcdir)/util/textdat.py

langall_eu_ETBLS = \
    $(top_srcdir)/conf/char_encoding_tbl_eu.txt

langalleng_DATFILES_C = \
    eng/guitext.dat

langalleng_POFILES_C = \
    guitext_eng.pot

langallengdir = $(prefix)/$(PACKAGE)/language/eng
langalleng_DATA = $(langalleng_DATFILES_C)

$(langalleng_DATFILES_C): $(langalleng_POFILES_C)
	$(MKDIR_P) "$(@D)"
	$(TEXTDAT) -t "$(word 1,$(langall_eu_ETBLS))" -d $@ -c $+

langallfre_DATFILES_C = \
    fre/guitext.dat

langallfre_POFILES_C = \
    guitext_fre.po

langallfredir = $(prefix)/$(PACKAGE)/language/fre
langallfre_DATA = $(langallfre_DATFILES_C)

$(langallfre_DATFILES_C): $(langallfre_POFILES_C)
	$(MKDIR_P) "$(@D)"
	$(TEXTDAT) -t "$(word 1,$(langall_eu_ETBLS))" -d $@ -c $+

langallger_DATFILES_C = \
    ger/guitext.dat

langallger_POFILES_C = \
    guitext_ger.po

langallgerdir = $(prefix)/$(PACKAGE)/language/ger
langallger_DATA = $(langallger_DATFILES_C)

$(langallger_DATFILES_C): $(langallger_POFILES_C)
	$(MKDIR_P) "$(@D)"
	$(TEXTDAT) -t "$(word 1,$(langall_eu_ETBLS))" -d $@ -c $+

langallita_DATFILES_C = \
    ita/guitext.dat

langallita_POFILES_C = \
    guitext_ita.po

langallitadir = $(prefix)/$(PACKAGE)/language/ita
langallita_DATA = $(langallita_DATFILES_C)

$(langallita_DATFILES_C): $(langallita_POFILES_C)
	$(MKDIR_P) "$(@D)"
	$(TEXTDAT) -t "$(word 1,$(langall_eu_ETBLS))" -d $@ -c $+

langallpol_DATFILES_C = \
    pol/guitext.dat

langallpol_POFILES_C = \
    guitext_pol.po

langallpoldir = $(prefix)/$(PACKAGE)/language/pol
langallpol_DATA = $(langallpol_DATFILES_C)

$(langallpol_DATFILES_C): $(langallpol_POFILES_C)
	$(MKDIR_P) "$(@D)"
	$(TEXTDAT) -t "$(word 1,$(langall_eu_ETBLS))" -d $@ -c $+

langallspa_DATFILES_C = \
    spa/guitext.dat

langallspa_POFILES_C = \
    guitext_spa.po

langallspadir = $(prefix)/$(PACKAGE)/language/spa
langallspa_DATA = $(langallspa_DATFILES_C)

$(langallspa_DATFILES_C): $(langallspa_POFILES_C)
	$(MKDIR_P) "$(@D)"
	$(TEXTDAT) -t "$(word 1,$(langall_eu_ETBLS))" -d $@ -c $+

EXTRA_DIST = \
	langallfre \
	langallger \
	langallita \
	langallpol \
	langallspa \
	langalleng
