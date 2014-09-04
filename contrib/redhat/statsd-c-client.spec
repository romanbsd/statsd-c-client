Name:		statsd-c-client
Version:	2.0.1
Release:	1%{?dist}
Summary:	Statsd C client library

Group:		System Environment/Daemons
License:	MIT
URL:		https://github.com/romanbsd/statsd-c-client
Source0:	%{name}-%{version}.tar.bz2
BuildRoot:	%(mktemp -ud %{_tmppath}/%{name}-%{version}-%{release}-XXXXXX)

BuildRequires:	glibc
Requires:	glibc

%description
Client library that implements the statsd protocol

%package devel
Summary:	Development package for statsd client
Group:		System Environment/Daemons

%description devel
Header files of the statsd client package

%prep
%setup -q -n %{name}

%build
make %{?_smp_mflags}

%install
rm -rf %{buildroot}
mkdir -p %{buildroot}%{_libdir}
mkdir -p %{buildroot}%{_docdir}/%{name}
mkdir -p %{buildroot}%{_includedir}/%{name}
cp -f libstatsdclient.so.%{version} %{buildroot}%{_libdir}
cp -f README.md %{buildroot}%{_docdir}/%{name}
cp -f LICENSE %{buildroot}%{_docdir}/%{name}
cp -f statsd-client.h %{buildroot}%{_includedir}/%{name}

%clean
rm -rf %{buildroot}

%files
%defattr(-,root,root,-)
%{_libdir}
%{_docdir}

%files devel
%defattr(-,root,root,-)
%{_includedir}

%changelog
* Fri Aug 01 2014 Sandeep Shandilya <sanshand@cisco.com> 2.0.1-1
- Create client as a shared lib
- the versioning is same as the python-statsd on Ubuntu-14.04
