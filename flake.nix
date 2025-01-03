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
            clang-tools
            clang
            libGL
            bear
            entr

            xorg.libX11
            xorg.libX11.dev
            xorg.libXcursor
            xorg.libXi
            xorg.libXinerama
            xorg.libXrandr
            wayland
          ];
          nativeBuildInputs = with pkgs; [
            clang-tools
            clang
            llvmPackages_latest.lldb
            llvmPackages_latest.libllvm
            llvmPackages_latest.libcxx
            llvmPackages_latest.clang
          ];

          shellHook = ''
            export LIBCLANG_PATH=${pkgs.libclang.lib}/lib
          '';

        };
      }
    );
}
