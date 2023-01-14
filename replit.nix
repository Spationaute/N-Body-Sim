{ pkgs }: {
	deps = [
		pkgs.clang_12
		pkgs.cmake
		pkgs.gdb
        pkgs.armadillo
        pkgs.jsoncpp
        pkgs.zip
	];
}