tokei --files --output json | jq -r '
           to_entries[] | select(.key != "Total") | .value.reports[]? |
           "\(.stats.code)\t\(.name)"
         ' | sort -rn | head -10