{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  name = "laputa";

  buildInputs = [
    pkgs.pkg-config
    pkgs.SDL2
    pkgs.SDL2_image
    pkgs.clang-tools
  ];

  shellHook = ''
    echo "laputa shell is ready"
  '';
}
