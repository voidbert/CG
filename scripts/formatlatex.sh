#!/bin/sh

# This script checks for formatting errors in the LaTeX reports.

# Copyright 2025 Ana Oliveira, Humberto Gomes, Mariana Rocha, Sara Lopes
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

. "$(dirname "$0")/utils.sh"

# shellcheck disable=SC2266
find reports -type f -name "*.tex" | while IFS="" read -r file; do
	i=1
	while IFS="" read -r line; do
		if printf "%s" "$line" | grep -Pq '\t'; then
            printf "%s:%s: Use of tabs: \"%s\"\n" "$file" "$i" "$line"
		fi

		if printf "%s" "$line" | grep -Pq '[\t ]$'; then
            printf "%s:%s: Trailing whitespace: \"%s\"\n" "$file" "$i" "$line"
		fi

		if [ "$(printf "%s" "$line" | wc -m)" -gt 100 ]; then
			printf "%s:%s: Column limit of 100 surpassed: \"%s\"\n" "$file" "$i" "$line"
		fi

		i=$((i + 1))
	done < "$file"
done | tee /dev/stderr | [ "$(wc -l)" = 0 ]
