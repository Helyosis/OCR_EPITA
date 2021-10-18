with import <nixpkgs> {};

let
    stdenv8 = overrideCC stdenv gcc8;
in
    stdenv8.mkDerivation rec {
        name = "sowon-build";
        env = buildEnv {
            name = name;
            paths = buildInputs;
        };
        buildInputs = [
            pkgconfig
            SDL2
            SDL2_image
            SDL2_gfx
	    opencv
        ];
    }
