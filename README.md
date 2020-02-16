# fegeya-os
FegeyaOS Deposu


# İnşa etmek:

# Paketleri yükleyin: 

Ubuntu/Debian = sudo apt install git nasm autoconf automake texinfo flex bison gawk build-essential unzip wget zip less zlib1g-dev xorriso libtool mtools gcc-multilib python3

Fedora = sudo yum install git nasm autoconf automake texinfo flex bison gcc gcc-c++ make glibc-devel zlib-devel xorriso curl-devel byacc libstdc++-static mtools glibc-devel.i686 libstdc++-devel.i686 python36

Gentoo = sudo emerge -av dev-vcs/git autoconf automake texinfo flex bison gawk tar sys-libs/zlib libisoburn wget nasm net-misc/curl bc mtools

openSUSE = sudo zypper install git nasm autoconf automake texinfo flex bison gcc-c++ make glibc-devel zlib-devel curl-devel xorriso python3

Arch = sudo pacman -S base-devel bison git texinfo nasm openssh unzip curl wget flex xorriso bc mtools python

FreeBSD = sudo pkg install bison git nasm gawk texinfo xorriso wget u-boot-tools mtools linuxfdisk curl python3 gcc gmake


# Konfigure et:

mkdir generated.x86_64; cd generated.x86_64
../configure --build-cross-tools x86_64 ../../buildtools

# İnşa et:

jam -q -j2 @nightly-anyboot




Fusion, GPLv3 ile lisanslanmıştır.
Fusion, Haiku'yu baz alır.
