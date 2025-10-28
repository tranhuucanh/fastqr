# Contributing to FastQR

Thank you for your interest in contributing to FastQR! This document provides guidelines and instructions for contributing.

## 🤝 How to Contribute

### Reporting Bugs

1. Check if the bug has already been reported in [Issues](https://github.com/tranhuucanh/fastqr/issues)
2. If not, create a new issue with:
   - Clear title and description
   - Steps to reproduce
   - Expected vs actual behavior
   - Your environment (OS, compiler, library versions)
   - Code samples or test cases

### Suggesting Features

1. Check [Issues](https://github.com/tranhuucanh/fastqr/issues) for similar requests
2. Create a new issue tagged with "enhancement"
3. Describe the feature and its use case
4. Explain why it would be useful

### Pull Requests

1. Fork the repository
2. Create a feature branch: `git checkout -b feature/amazing-feature`
3. Make your changes
4. Add tests for new functionality
5. Ensure all tests pass: `make test`
6. Commit with clear messages: `git commit -m 'Add amazing feature'`
7. Push to your fork: `git push origin feature/amazing-feature`
8. Open a Pull Request

## 📝 Development Guidelines

### Code Style

- **C++**: Follow Google C++ Style Guide
- **Ruby**: Follow Ruby Style Guide
- **JavaScript**: Follow Airbnb JavaScript Style Guide
- **PHP**: Follow PSR-12

### Commit Messages

- Use present tense: "Add feature" not "Added feature"
- Be descriptive but concise
- Reference issues: "Fix #123: Description"

### Testing

- Add tests for all new features
- Ensure existing tests pass
- Test on multiple platforms if possible

## 🏗️ Development Setup

Build the project locally:

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
make
```

## 📄 License

By contributing, you agree that your contributions will be licensed under LGPL 2.1.

