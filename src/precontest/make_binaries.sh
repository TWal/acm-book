mkdir ~/bin
echo -e '#!/bin/bash\ng++ -Wall -Wextra -O2 -std=c++14 "$@"' > ~/bin/g++std
echo -e '#!/bin/bash\ng++ -Wall -Wextra -O0 -std=c++14 -g -DDEBUG "$@"' \
	> ~/bin/g++dbg
