{
  description = "A basic flake with a shell";
  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
  inputs.systems.url = "github:nix-systems/default";
  inputs.flake-utils = {
    url = "github:numtide/flake-utils";
    inputs.systems.follows = "systems";
  };

  outputs =
    {
      nixpkgs,
      flake-utils,
      ...
    }:
    flake-utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = nixpkgs.legacyPackages.${system};

      in
      {
        devShells.default = pkgs.mkShell {
          packages = with pkgs; [
            alsa-lib
            bacon
            bear
            cargo
            cargo-deny
            cargo-edit
            cargo-watch
            clang
            clang-tools
            cmake
            dockerfile-language-server-nodejs
            glfw-wayland
            glibc
            gnumake
            libGL
            libatomic_ops
            libclang
            libgcc
            libxkbcommon
            markdownlint-cli2
            mesa
            openssl
            pkg-config
            raylib
            rust-analyzer
            rustc
            rustfmt
            rustup
            wayland
            wayland-protocols
            wayland-scanner
            xorg.libX11
            xorg.libX11.dev
            xorg.libXcursor
            xorg.libXft
            xorg.libXi
            xorg.libXinerama
            xorg.libXrandr

          ];
          nativeBuildInputs = with pkgs; [
            clang
            clang-tools
            entr
            libxkbcommon
            wayland
            wayland-protocols
            ecm
            glfw
            glfw-wayland
            glfw3
            libGL

            xorg.libX11
            xorg.libX11.dev
            xorg.libXcursor
            xorg.libXi
            xorg.libXinerama
            xorg.libXrandr

          ];

          shellHook = ''
            export PATH=$HOME/go/bin:$PATH
            export LIBCLANG_PATH=${pkgs.libclang.lib}/lib
            # export CMAKE_PREFIX_PATH=${pkgs.glfw3}/lib/cmake/glfw3
          '';

        };
      }
    );
}
