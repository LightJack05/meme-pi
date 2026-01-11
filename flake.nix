{
  description = "Cross-compile environment for RPi Alpine Kernel Modules";

  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
  };

  outputs = { self, nixpkgs }:
    let
      system = "x86_64-linux";
      pkgs = nixpkgs.legacyPackages.${system};
      
      # The cross-compilation toolchain for AArch64
      crossPkgs = pkgs.pkgsCross.aarch64-multiplatform;
    in
    {
      devShells.${system}.default = pkgs.mkShell {
        # Host dependencies (tools needed to run the build system on your Desktop)
        nativeBuildInputs = [
          pkgs.gnumake
          pkgs.flex
          pkgs.bison
          pkgs.openssl.dev
          pkgs.ncurses
          pkgs.bear
          
          # The Cross Compiler (runs on x86, builds for ARM64)
          crossPkgs.buildPackages.gcc
        ];

        # Environment variables for Kbuild
        # Nix places the cross-compiler in the path. We just need to tell Kbuild the prefix.
        ARCH = "arm64";
        CROSS_COMPILE = "aarch64-unknown-linux-gnu-";

        shellHook = ''
          echo "🌲 Alpine RPi Cross-Compile Environment Loaded"
          echo "Target: AArch64 (Raspberry Pi)"
          echo "Compiler: $(which aarch64-unknown-linux-gnu-gcc)"
        '';
      };
    };
}
