# Jira issue-creation prompt for claude.ai

This repo pairs Claude Code (local, in-IDE) with claude.ai (web, Atlassian MCP
enabled) for issue tracking. Claude Code does not have direct Jira access;
claude.ai does. Paste one of the prompts below into a claude.ai chat to create
an EBR2 issue, then bring the returned issue key back to the Claude Code
session to name the branch and commits.

**Target:** [EBR2 board](https://hbpneurorobotics.atlassian.net/jira/software/c/projects/EBR2/boards/11)
on `hbpneurorobotics.atlassian.net`.

## Full prompt — use for real bugs and non-trivial improvements

Fill in the `<bracketed>` fields before sending.

```text
Create a Jira issue in the hbpneurorobotics Atlassian site,
project key EBR2 (board 11, https://hbpneurorobotics.atlassian.net/jira/software/c/projects/EBR2/boards/11).

Cloud site: hbpneurorobotics.atlassian.net
Project: EBR2
Issue type: {Bug | Task | Improvement}     ← pick one
Summary: <one-line title, ≤ 120 chars>
Priority: {Highest | High | Medium | Low}  ← default Medium
Labels: nrp-core

Description (use Jira Markdown / ADF):

## Context
<what the issue is about — one paragraph>

## Current behavior
<what happens today, with file:line references when relevant>

## Expected behavior
<what should happen>

## Proposed fix / acceptance criteria
- [ ] <criterion 1>
- [ ] <criterion 2>
- [ ] Unit test suite in `nrp-local/nrp-nest-gazebo-ubuntu20:local`
      stays green (`bash .ci/30-run-tests.sh` exits 0)

## Branch / commit convention
- Branch: `EBR2-<key>-<short-slug>` (to be filled in after issue creation)
- Commits: `[EBR2-<key>] <imperative summary>`

After creating the issue, reply with:
1. The full issue key (e.g. EBR2-234)
2. The issue URL
3. A suggested branch name of the form `EBR2-<num>-<short-slug>`
```

## Short prompt — for small fixes

```text
In hbpneurorobotics.atlassian.net, create an EBR2 {Bug|Task|Improvement}
titled "<summary>" with labels nrp-core. Description:
<1-3 sentences>. Return the issue key, URL, and a suggested branch name
`EBR2-<num>-<slug>`.
```

## Notes

- If claude.ai asks you which site, answer `hbpneurorobotics.atlassian.net`.
- If it asks which project, answer `EBR2`.
- If it proposes an issue type of `Story` by default, override to `Bug` /
  `Task` / `Improvement` — the board uses those three for code work.
- If the Atlassian MCP returns an error, paste the error back into the chat
  and ask it to retry; don't manually create the issue in the Jira web UI and
  skip the MCP path, or the issue URL won't flow back into the session.
