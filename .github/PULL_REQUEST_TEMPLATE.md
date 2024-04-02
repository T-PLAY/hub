# Pull Request Desription

## Check if you branch history is PR compatible

- Your branch need to be up to date with origin/main
- Update your git repository `git fetch origin` if origin is this remote
- Check format style with the script provided in `scripts/source-format-all.sh`
- You must use clang-format style (github CI version is 11)
- Your proposal must include a commit where the file CHANGELOG is updated (add a line describing the proposed change + PR id, see file for examples). If your contribution is significant, you can also ask to be added to the contributor list in the discussion.
_These checks are enforced by github workflow actions_
_Please refer to the corresponding log in case of failure_

## UPDATE the form below to describe your PR

Please check if the PR fulfills these requirements

- [ ] The commit message follows our guidelines (see CONTRIBUTING)
- [ ] Tests for the changes have been added (for bug fixes / features)
- [ ] Docs have been added / updated (for bug fixes / features)

Be aware that the PR request cannot be accepted if it doesn't pass the Continuous Integration tests.

- **What kind of change does this PR introduce?**
  - [ ] bug fix
  - [ ] feature
  - [ ] docs update
  - [ ] other:

- **What is the current behavior?** (You can also link to an open issue here)

- **What is the new behavior (if this is a feature change)?**

- **Does this PR introduce a breaking change?** (What changes might users need to make in their application due to this PR?)

- **Other information**:
