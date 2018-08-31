#/bin/sh

usage_err() {
  >&2 echo "Usage: $0 [EXPORT_DIR]"
  exit 1
}

if [ $# -gt 1 ]; then
  usage_err
fi

if [ $# -eq 0 ]; then
  outdir="."
else
  if [ -e "$1" ]; then
    if [ ! -d "$1" ]; then
      >&2 echo "Not a directory: $1"
      usage_err
    fi
  else
    mkdir "$1"
  fi

  outdir="$1"
fi

for e in exercise*; do
  tar --exclude-from=.tarignore -vczf "$outdir/$e.tar.gz" $e
done
