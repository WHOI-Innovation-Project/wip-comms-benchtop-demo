## Reuse (and reusability)

Open source software, when developed in an ecosystem such as Git+Github/Gitlab/Bitbucket, provides social mechanisms for reuse.

However, code is only suitable for reuse when reusability has been considered in the design of the original software. This is a non-trivial amount of effort, and is often forgotten or dropped in the rush to deliver a project for a specific application.

Successful reuseability is focused around thinking about these ideas for your project:

  - Generalization (this is inherently intension with application-specificity - some balance must be struck). In C++ this is often realized via generic programming (templates) or polymorphism (either runtime inheritance or compile-time inheritance).
  - Modularity - arguable one of the largest values of a publish/subscribe middleware is to allow significant modularity of the system. The only shared information between modules (whether they are threads, processes, or entire vehicles) is the data published and the data subscribed to.
  - Clarity - straightforward and conventions/standards-compliant APIs. If there is no convention, be consistent within a project.
  - Documentation - Examples are provided ("tutorial") and APIs are documented. Well designed projects can be unapproachable and unsuited for reuse simply because there is no realistic learning entry into the codebase for the outsider.
  - Legal - Open source licensing models provide a legal framework for acceptable reuse in most conceivable domain areas (but *do* place restrictions that should be considered).
  - Programming language - software written in a common language for the application area (e.g. C/C++ for embedded systems, Python for data processing and end-user systems, JavaScript for web applications) is more suitable for reuse, as there are more experts in that area who know how to use that language.
  - Deployment- Having accessible packages for the operating systems commonly in use for the application (e.g. Debian/Ubuntu Linux) ensures the that the potential reuser doesn't have to build your project from source (which is often trickier for them than for you!). Can your users "sudo apt install ..." your project?