desktopdir = $(datadir)/applications
desktop_in_files = gelemental.desktop.in
desktop_DATA = $(desktop_in_files:.desktop.in=.desktop)
@INTLTOOL_DESKTOP_RULE@

iconsdir = $(datadir)/icons/hicolor
svgicondir = $(iconsdir)/scalable/apps
dist_svgicon_DATA = scalable/gelemental.svg
icon16dir = $(iconsdir)/16x16/apps
dist_icon16_DATA = 16x16/gelemental.png
icon22dir = $(iconsdir)/22x22/apps
dist_icon22_DATA = 22x22/gelemental.png
icon24dir = $(iconsdir)/24x24/apps
dist_icon24_DATA = 24x24/gelemental.png
icon32dir = $(iconsdir)/32x32/apps
dist_icon32_DATA = 32x32/gelemental.png
icon48dir = $(iconsdir)/48x48/apps
dist_icon48_DATA = 48x48/gelemental.png

gtk_update_icon_cache = gtk-update-icon-cache -f -t $(datadir)/icons/hicolor
install-data-hook: update-icon-cache
uninstall-hook: update-icon-cache
update-icon-cache:
	@-if test -z "$(DESTDIR)"; then \
		echo "Updating Gtk icon cache."; \
		$(gtk_update_icon_cache); \
	else \
		echo "*** Icon cache not updated. After (un)install, run this:"; \
		echo "***   $(gtk_update_icon_cache)"; \
	fi

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_in_files = libelemental.pc.in
pkgconfig_DATA = $(pkgconfig_in_files:.pc.in=.pc)

EXTRA_DIST = \
	$(desktop_in_files) \
	$(pkgconfig_in_files) \
	libelemental-uninstalled.pc.in

CLEANFILES = $(desktop_DATA)
