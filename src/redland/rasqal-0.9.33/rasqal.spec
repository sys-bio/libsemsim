# -*- RPM-SPEC -*-
%define name    rasqal
%define version 0.9.33
%define release SNAP

Name:      %{name}
Version:   %{version}
Release:   %{release}
Summary:   Rasqal RDF Query Library

Group:     System Environment/Libraries
License:   LGPLv2+ or ASL 2.0
Source:    http://download.librdf.org/source/%{name}-%{version}.tar.gz
URL:       http://librdf.org/rasqal/
BuildRoot: /tmp/%{name}-%{version}

BuildRequires: libxml2-devel
BuildRequires: mpfr-devel
BuildRequires: pcre-devel
BuildRequires: raptor2-devel >= 2.0.7
# for the testsuite
BuildRequires:  perl(XML::DOM)

Packager:  Dave Beckett <dave@dajobe.org>

# Not needed since raptor2-devel require above will pull this in.
# Requires: raptor2 >= 2.0.7

%description

Rasqal is a library providing full support for querying Resource
Description Framework (RDF) including parsing query syntaxes, constructing
the queries, executing them and returning result formats.  It supports
parsing and executing the SPARQL query language.

%package devel
Summary: Development files for the Rasqal RDF query library
Group: Development/Libraries
Requires: rasqal = %{version}
Requires: gtk-doc
Requires: pkgconfig
Requires: raptor2-devel >= 2.0.7
%description devel
Libraries, includes etc to develop with the Rasqal RDF query language library.

%prep
%setup -q

%build

%configure --enable-release

%{__make} OPTIMIZE="$RPM_OPT_FLAGS"

%install
[ "$RPM_BUILD_ROOT" != "/" ] && rm -rf $RPM_BUILD_ROOT

install -d $RPM_BUILD_ROOT%{_mandir}/man1
install -d $RPM_BUILD_ROOT%{_mandir}/man3

%makeinstall

# Fedora deletes .la files here

%clean
[ -n "$RPM_BUILD_ROOT" -a "$RPM_BUILD_ROOT" != / ] && rm -rf $RPM_BUILD_ROOT

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig

%files
%defattr(-, root, root)
%doc AUTHORS COPYING COPYING.LIB ChangeLog LICENSE.txt NEWS README
%doc LICENSE-2.0.txt NOTICE
%doc *.html
%{_bindir}/roqet
%{_libdir}/librasqal*.so.*
%doc %{_mandir}/man1/roqet.1*

%files devel
%defattr(-, root, root)
%doc docs/README.html
%{_bindir}/rasqal-config
%{_includedir}/rasqal/
%{_libdir}/librasqal*.so
%{_libdir}/pkgconfig/rasqal.pc
%{_mandir}/man1/rasqal-config.1*
%{_mandir}/man3/librasqal.3*
%{_datadir}/gtk-doc/html/rasqal/

# Fedora rasqal-devel package does not include these
%{_libdir}/librasqal*.a
%{_libdir}/librasqal*.la


%changelog
* Tue Feb 1 2011  Dave Beckett <dave@dajobe.org>
- Raptor 2 attempt

* Wed Aug 11 2005  Dave Beckett <dave.beckett@bristol.ac.uk>
- Update Source:
- Use %makeinstall

* Wed Aug 10 2005  Dave Beckett <dave.beckett@bristol.ac.uk>
- Use %configure.

* Fri Jul 28 2005  Dave Beckett <dave.beckett@bristol.ac.uk>
- Updated for gtk-doc locations

* Fri Oct 22 2004 <Dave.Beckett@bristol.ac.uk>
- License now LGPL/Apache 2
- Added LICENSE-2.0.txt and NOTICE

* Wed May 5 2004 <Dave.Beckett@bristol.ac.uk>
- Ship roqet and roqet.1

* Sat May 1 2004 <Dave.Beckett@bristol.ac.uk>
- Requires raptor 1.3.0

* Mon Feb 24 2004 <Dave.Beckett@bristol.ac.uk>
- Requires raptor

* Mon Aug 11 2003 <Dave.Beckett@bristol.ac.uk>
- Initial packaging
