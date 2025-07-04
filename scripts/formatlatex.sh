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

grep_error_message() {
    sed -r "s/^([^:]*):([^:]*):.*$/\2:\1: $1/g"
}

# shellcheck disable=SC2266
find reports -type f -name "*.tex" | while IFS="" read -r file; do
    grep -PHn '.{101,}$' "$file" | \
        grep -Pv 'http' | \
        grep_error_message "Column exceeds 100 characters"
    grep -PHn '\t$'      "$file" | grep_error_message "Use of tabs"
    grep -PHn '\s+$'     "$file" | grep_error_message "Trailing whitespace"
done |
    sort -n |
    sed -r "s/^([^:]*):([^:]*):(.*)$/\2:\1:\3/g" |
    tee /dev/stderr |
    test "$(wc -l)" = 0
