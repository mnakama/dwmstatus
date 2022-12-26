with import <nixpkgs> {};
stdenv.mkDerivation {
  name = "dwmstatus";
  buildInputs = [ xlibsWrapper ];
}
