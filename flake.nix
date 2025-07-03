{
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
  };
  outputs = { nixpkgs, ... }: let
    forAllSystems = function: 
      nixpkgs.lib.genAttrs [
        "x86_64-linux"
        "aarch64-linux"
      ] (system: function nixpkgs.legacyPackages."${system}");
  in {
    devShells = forAllSystems (pkgs: {
      default = pkgs.mkShell {
        nativeBuildInputs = with pkgs; [
          cmake python3Minimal
        ];
        hardeningDisable = [ "all" ];
        };
    });
    packages = forAllSystems (pkgs: {
      default = pkgs.stdenv.mkDerivation {
        pname = "MinimalSocket";
        version = "9999";

        src = ./.;
        strictDeps = true;
        cmakeFlags = [
          (pkgs.lib.strings.cmakeBool "BUILD_MinimalCppSocket_SAMPLES" false)
          (pkgs.lib.strings.cmakeBool "BUILD_MinimalCppSocket_TESTS" false)
          (pkgs.lib.strings.cmakeBool "LIB_OPT" true)
        ];

        nativeBuildInputs = with pkgs; [
          cmake
          python3Minimal
        ];

        meta = {
          homepage = "https://github.com/andreacasalino/Minimal-Socket";
          description = "Minimal cross platform C++ tcp, udp socket implementation";
          license = pkgs.lib.licenses.gpl3;
          platforms = pkgs.lib.platforms.unix;
        };

      };
    });
  };
}
