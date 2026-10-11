+ The server is strict about the correct relative path being for serving files. For example, `F001.dat` needs to be `data/F001.dat`, because only `"data/F001.dat"` is mapped its authentication toke in `file.json`. `cgrab` automatically retries once, tacking the `data/` prefix on if missing.

+ Logging all goes to standard output for now
