#include "clar_libgit2.h"
#include "posix.h"
#include "path.h"
#include "submodule_helpers.h"
#include "fileops.h"

static git_repository *g_repo = NULL;

void test_submodule_repository_init__basic(void)
{
	git_submodule *sm;
	git_repository *repo;
	git_config *cfg;
	const char *worktree_path;
	git_buf dot_git_content = GIT_BUF_INIT;

	g_repo = setup_fixture_submod2();

	cl_git_pass(git_submodule_lookup(&sm, g_repo, "sm_gitmodules_only"));
	cl_git_pass(git_submodule_init(sm, 0));
	cl_git_pass(git_submodule_repo_init(&repo, sm, 1));

	/* Verify worktree */
	cl_git_pass(git_repository_config(&cfg, repo));
	cl_git_pass(git_config_get_string(&worktree_path, cfg, "core.worktree"));
	cl_assert_equal_s("../../../sm_gitmodules_only/", worktree_path);

	/* Verify gitlink */
	cl_git_pass(git_futils_readbuffer(&dot_git_content, "submod2/" "sm_gitmodules_only" "/.git"));
	cl_assert_equal_s("gitdir: ../.git/modules/sm_gitmodules_only/", dot_git_content.ptr);

	cl_assert(git_path_isfile("submod2/" "sm_gitmodules_only" "/.git"));

	cl_assert(git_path_isdir("submod2/.git/modules"));
	cl_assert(git_path_isdir("submod2/.git/modules/" "sm_gitmodules_only"));
	cl_assert(git_path_isfile("submod2/.git/modules/" "sm_gitmodules_only" "/HEAD"));

	git_config_free(cfg);
	git_submodule_free(sm);
	git_repository_free(repo);
	git_buf_free(&dot_git_content);
}
