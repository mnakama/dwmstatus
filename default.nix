with import <nixpkgs> {};
stdenv.mkDerivation {
  name = "dwmstatus";
  buildInputs = [ xorg.libX11 ];
}
