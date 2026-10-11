## Downloading this repository

Go to the topmost [README.md](https://github.com/Matias-G-Toro/FA26-CSE-30264-mtoro/tree/main)

## Testing - My Repository

+ Navigate to `cp1/p1`.
+ Type `make` to create `cgrab` on `student10.cse.nd.edu` or any other student machine.

## Testing - Class repository component

+ Outside of this repo download the class repository and proper scripts for testing:

```bash
git clone git@github.com:adstriegel/cse30264-fa26-class.git
```

+ Navigate to `homework/cp1/part1/grab`.
+ On a separate terminal (or pseudoterminal like tmux), run the command:

```python
python3 server-grab.py --port 54000 ../files.json --prefix ../
```
+ NOTE: To test robustnesss, add an empty and 100 MiB file. Unfortunately, I cannot attach such a large file to GitHub nor can I attach an empty file

## Remarks

+ Logging all goes to standard output for now

+ The server is strict about the relative path for serving files. For example, `F001.dat` needs to be `data/F001.dat` in a client request, because only `"data/F001.dat"` is mapped its authentication toke in `file.json`. `cgrab` automatically retries once, tacking the `data/` prefix on if missing.
