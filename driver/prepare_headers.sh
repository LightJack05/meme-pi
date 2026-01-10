#!/usr/bin/env bash
set -e

# --- Configuration (Match these to your target Alpine version) ---
KERN_VER="6.12"
PATCH_VER="62"
PKG_REL="0"
ALPINE_FLAVOR="rpi"
ALPINE_MIRROR="https://dl-cdn.alpinelinux.org/alpine/latest-stable/main/aarch64"
MAKEFLAGS="-j$(nproc)"

# --- Derived Vars ---
FULL_VER="${KERN_VER}.${PATCH_VER}"
WORK_DIR="kernel-headers.d"
SRC_DIR="${WORK_DIR}/linux-${KERN_VER}"
DEV_APK="linux-${ALPINE_FLAVOR}-dev-${FULL_VER}-r${PKG_REL}.apk"

# Ensure ARCH and CROSS_COMPILE are set (from your Nix flake)
if [ -z "$ARCH" ] || [ -z "$CROSS_COMPILE" ]; then
    echo "Error: ARCH or CROSS_COMPILE not set."
    exit 1
fi

mkdir -p "$WORK_DIR"
cd "$WORK_DIR"

echo ">>> 1. Downloading resources..."
wget -nc "https://cdn.kernel.org/pub/linux/kernel/v${KERN_VER%%.*}.x/linux-${KERN_VER}.tar.xz"
wget -nc "https://cdn.kernel.org/pub/linux/kernel/v${KERN_VER%%.*}.x/patch-${FULL_VER}.xz"
wget -nc "https://dev.alpinelinux.org/archive/rpi-patches/rpi-${FULL_VER}-alpine.patch"
wget -nc "https://gitlab.alpinelinux.org/alpine/aports/-/raw/master/main/linux-rpi/common-changes.config"
wget -nc "${ALPINE_MIRROR}/${DEV_APK}"

echo ">>> 2. Extracting and Patching..."
if [ ! -d "linux-${KERN_VER}" ]; then
    tar -xf "linux-${KERN_VER}.tar.xz"
fi

cd "linux-${KERN_VER}"

if [ ! -f ".patched_alpine" ]; then
    echo "Applying patches..."
    xzcat "../patch-${FULL_VER}.xz" | patch -p1
    patch -p1 < "../rpi-${FULL_VER}-alpine.patch"
    touch .patched_alpine
fi

echo ">>> 3. configuring..."
cp -v arch/arm64/configs/bcm2711_defconfig .config

# Set Alpine localversion
echo "-${PKG_REL}-${ALPINE_FLAVOR}" > localversion-alpine

# Apply common-changes.config using kernel script (Logic adapted from APKBUILD)
while read -r line; do
    # Skip comments and empty lines
    [[ "$line" =~ ^#.* ]] && continue
    [[ -z "$line" ]] && continue
    
    opt="${line%%=*}"
    val="${line#*=}"
    
    # Execute directly with proper quoting
    case "$val" in
        y)
            ./scripts/config --file .config --enable "$opt"
            ;;
        n)
            ./scripts/config --file .config --disable "$opt"
            ;;
        m)
            ./scripts/config --file .config --module "$opt"
            ;;
        \"*)
            # Strip quotes and handle empty string correctly
            clean_val="${val//\"/}"
            ./scripts/config --file .config --set-str "$opt" "$clean_val"
            ;;
        *)
            # set-val for numbers/hex
            ./scripts/config --file .config --set-val "$opt" "$val"
            ;;
    esac
done < "../common-changes.config"

# Finalize config
make $MAKEFLAGS olddefconfig

echo ">>> 4. Preparing build tools (modules_prepare)..."
make $MAKEFLAGS modules_prepare

echo ">>> 5. Installing Module.symvers from APK..."
cd ..
# Extract APK (it's a tarball)
tar -xf "$DEV_APK"
# Copy symvers to kernel root
cp "usr/src/linux-headers-${FULL_VER}-${PKG_REL}-${ALPINE_FLAVOR}/Module.symvers" "linux-${KERN_VER}/"

echo ">>> Done. Kernel source ready at: $PWD/linux-${KERN_VER}"
