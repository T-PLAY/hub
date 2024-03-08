#! /bin/bash -e


git filter-branch --msg-filter 'sed -E "s/Former-commit-id: [0-9a-f]{40}//g"' --tag-name-filter cat -- --all
