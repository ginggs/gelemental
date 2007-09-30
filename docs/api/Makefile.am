doxygen_configfile = Doxyfile
doxygen_configfile_source = $(srcdir)/Doxyfile.in
docdir = $(datadir)/doc/@PACKAGE@-@VERSION@
apidir = $(docdir)/api

EXTRA_DIST = Doxyfile.in

html/index.html: $(doxygen_configfile_source)
	-rm -rf html
	SRCDIR=$(srcdir) TOP_SRCDIR=$(top_srcdir) doxygen $(doxygen_configfile)

install-api: $(srcdir)/html/index.html
	@$(NORMAL_INSTALL)
	$(mkinstalldirs) $(DESTDIR)/$(apidir)
	@dir='$(<D)'; for p in $$dir/*.html $$dir/*.css $$dir/*.png $$dir/*.gif $$dir/*.dot ; do \
		f="`echo $$p | sed -e 's|^.*/||'`"; \
		echo " $(INSTALL_DATA) $$p $(DESTDIR)$(apidir)/$$f"; \
		$(INSTALL_DATA) $$p $(DESTDIR)$(apidir)/$$f; \
	done

uninstall-api: $(srcdir)/html/index.html
	@$(NORMAL_UNINSTALL)
	@dir='$(<D)'; for p in $$dir/*.html $$dir/*.css $$dir/*.png $$dir/*.gif $$dir/*.dot ; do \
		f="`echo $$p | sed -e 's|^.*/||'`"; \
		echo " rm -f $(DESTDIR)$(apidir)/$$f"; \
		rm -f $(DESTDIR)$(apidir)/$$f; \
	done

clean-api:
	-rm -rf $(srcdir)/html

all-local: html/index.html

install-data-local: install-api

uninstall-local: uninstall-api

maintainer-clean-local: clean-api

.PHONY: install-api uninstall-api clean-api
