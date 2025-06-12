# Contribution Guidelines

Thank you for your interest in contributing to this project! I truly appreciate all
the contributions from the community, whether it's proposing features, identifying bugs, or fixing mistakes! To ensure
that our collaboration is efficient and effective, please follow these guidelines.

Feel free to discuss or suggest improvements to this document
[by submitting a pull request](https://github.com/M3351AN/UkiaCS2/pulls).

## Ways to Contribute

There are multiple ways to contribute.

### Reporting an issue

Please [create an issue](https://github.com/M3351AN/UkiaCS2/issues/new/), assuming one does not already exist, and
describe your concern. Note you need a [GitHub account](https://github.com/signup/free) for this.

Clearly describe the issue:

- If it is a bug, please describe how to **reproduce** it. If possible, attach a _complete example_ which demonstrates
  the error. Please also state what you **expected** to happen instead of the error.
- If you propose a change or addition, try to give an **example** what the improved code could look like or how to use
  it.
- If you found a compilation error, please tell us which **compiler** (version and operating system) you used and paste
  the (relevant part of) the error messages to the ticket.

### Reporting a security vulnerability

You can report a security vulnerability according to our
[security policy](https://github.com/M3351AN/UkiaCS2/security/policy).

### Discussing a new feature

For questions, feature or support requests, please
[open a discussion](https://github.com/M3351AN/UkiaCS2/discussions/new). If you find a proposed answer satisfactory,
please use the "Mark as answer" button to make it easier for readers to see what helped and for the community to filter
for open questions.

### Proposing a fix or an improvement

Join an ongoing discussion or comment on an existing issue before starting to code. This can help to avoid duplicate
efforts or other frustration during the later review.

Create a [pull request](https://github.com/M3351AN/UkiaCS2/pulls)and follow the pull request template below.

- describe the changes in detail, both the what and why,
- reference existing issues where applicable,

We describe all points in detail below.

All contributions (including pull requests) must agree to the
[Developer Certificate of Origin (DCO) version 1.1](https://developercertificate.org). This is exactly the same one
created and used by the Linux kernel developers and posted on http://developercertificate.org/. This is a developer's
certification that he or she has the right to submit the patch for inclusion into the project.

## How to...

### Describe your changes

This library is primarily maintained as a spare-time project. As such, I cannot make any guarantee how quickly changes
are merged and released. Therefore, it is very important to make the review as smooth as possible by explaining not only
_what_ you changed, but _why_. This rationale can be very valuable down the road when improvements or bugs are discussed
years later.

### Reference an existing issue

[Link a pull request to an issue](https://docs.github.com/en/issues/tracking-your-work-with-issues/using-issues/linking-a-pull-request-to-an-issue)
to clarify that a fix is forthcoming and which issue can be closed after merging. Only a few cases (e.g., fixing typos)
do not require prior discussions.

## Recommended documentation

- The library’s [README file](README.md) is an excellent starting point to
  understand its functionality.

## Please don't...

Certain contributions are not helpful.

### Break C++23 language conformance

This software is designed to work with C++23 and later. This means that any compiler that support C++23 should compile
the software without problems. Some compilers like GCC 10 (and earlier), Clang 15 (and earlier), or MSVC 16.3.1 (and earlier) are known not to work due to missing or incomplete C++23 support.

Please do not add features that do not work with the mentioned supported compilers.

## Wanted

The following areas really need contribution and are always welcomed:

- New features/functions
- Improving performance

We look forward to your contributions and collaboration to enhance the software!
