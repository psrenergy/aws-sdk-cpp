﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/glue/Glue_EXPORTS.h>
#include <aws/glue/GlueRequest.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <aws/glue/model/SourceControlProvider.h>
#include <aws/glue/model/SourceControlAuthStrategy.h>
#include <utility>

namespace Aws
{
namespace Glue
{
namespace Model
{

  /**
   */
  class AWS_GLUE_API UpdateJobFromSourceControlRequest : public GlueRequest
  {
  public:
    UpdateJobFromSourceControlRequest();

    /**
     * Virtual Copy Constructor idiom;
     * Override in an inherited class to have overridden functions (such as GetRequestSpecificHeaders) be called by the SDK.
     * (If you are creating a child of this: don't forget to keep the original return type or (better) just use the 'override' keyword)
    */
    virtual Aws::UniquePtr<UpdateJobFromSourceControlRequest> Clone() const
    {
      return Aws::MakeUnique<UpdateJobFromSourceControlRequest>(GetServiceRequestName(), *this);
    }

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "UpdateJobFromSourceControl"; }

    Aws::String SerializePayload() const override;

    Aws::Http::HeaderValueCollection GetRequestSpecificHeaders() const override;


    /**
     * <p>The name of the Glue job to be synchronized to or from the remote
     * repository.</p>
     */
    inline const Aws::String& GetJobName() const{ return m_jobName; }

    /**
     * <p>The name of the Glue job to be synchronized to or from the remote
     * repository.</p>
     */
    inline bool JobNameHasBeenSet() const { return m_jobNameHasBeenSet; }

    /**
     * <p>The name of the Glue job to be synchronized to or from the remote
     * repository.</p>
     */
    inline void SetJobName(const Aws::String& value) { m_jobNameHasBeenSet = true; m_jobName = value; }

    /**
     * <p>The name of the Glue job to be synchronized to or from the remote
     * repository.</p>
     */
    inline void SetJobName(Aws::String&& value) { m_jobNameHasBeenSet = true; m_jobName = std::move(value); }

    /**
     * <p>The name of the Glue job to be synchronized to or from the remote
     * repository.</p>
     */
    inline void SetJobName(const char* value) { m_jobNameHasBeenSet = true; m_jobName.assign(value); }

    /**
     * <p>The name of the Glue job to be synchronized to or from the remote
     * repository.</p>
     */
    inline UpdateJobFromSourceControlRequest& WithJobName(const Aws::String& value) { SetJobName(value); return *this;}

    /**
     * <p>The name of the Glue job to be synchronized to or from the remote
     * repository.</p>
     */
    inline UpdateJobFromSourceControlRequest& WithJobName(Aws::String&& value) { SetJobName(std::move(value)); return *this;}

    /**
     * <p>The name of the Glue job to be synchronized to or from the remote
     * repository.</p>
     */
    inline UpdateJobFromSourceControlRequest& WithJobName(const char* value) { SetJobName(value); return *this;}


    /**
     * <p>The provider for the remote repository.</p>
     */
    inline const SourceControlProvider& GetProvider() const{ return m_provider; }

    /**
     * <p>The provider for the remote repository.</p>
     */
    inline bool ProviderHasBeenSet() const { return m_providerHasBeenSet; }

    /**
     * <p>The provider for the remote repository.</p>
     */
    inline void SetProvider(const SourceControlProvider& value) { m_providerHasBeenSet = true; m_provider = value; }

    /**
     * <p>The provider for the remote repository.</p>
     */
    inline void SetProvider(SourceControlProvider&& value) { m_providerHasBeenSet = true; m_provider = std::move(value); }

    /**
     * <p>The provider for the remote repository.</p>
     */
    inline UpdateJobFromSourceControlRequest& WithProvider(const SourceControlProvider& value) { SetProvider(value); return *this;}

    /**
     * <p>The provider for the remote repository.</p>
     */
    inline UpdateJobFromSourceControlRequest& WithProvider(SourceControlProvider&& value) { SetProvider(std::move(value)); return *this;}


    /**
     * <p>The name of the remote repository that contains the job artifacts.</p>
     */
    inline const Aws::String& GetRepositoryName() const{ return m_repositoryName; }

    /**
     * <p>The name of the remote repository that contains the job artifacts.</p>
     */
    inline bool RepositoryNameHasBeenSet() const { return m_repositoryNameHasBeenSet; }

    /**
     * <p>The name of the remote repository that contains the job artifacts.</p>
     */
    inline void SetRepositoryName(const Aws::String& value) { m_repositoryNameHasBeenSet = true; m_repositoryName = value; }

    /**
     * <p>The name of the remote repository that contains the job artifacts.</p>
     */
    inline void SetRepositoryName(Aws::String&& value) { m_repositoryNameHasBeenSet = true; m_repositoryName = std::move(value); }

    /**
     * <p>The name of the remote repository that contains the job artifacts.</p>
     */
    inline void SetRepositoryName(const char* value) { m_repositoryNameHasBeenSet = true; m_repositoryName.assign(value); }

    /**
     * <p>The name of the remote repository that contains the job artifacts.</p>
     */
    inline UpdateJobFromSourceControlRequest& WithRepositoryName(const Aws::String& value) { SetRepositoryName(value); return *this;}

    /**
     * <p>The name of the remote repository that contains the job artifacts.</p>
     */
    inline UpdateJobFromSourceControlRequest& WithRepositoryName(Aws::String&& value) { SetRepositoryName(std::move(value)); return *this;}

    /**
     * <p>The name of the remote repository that contains the job artifacts.</p>
     */
    inline UpdateJobFromSourceControlRequest& WithRepositoryName(const char* value) { SetRepositoryName(value); return *this;}


    /**
     * <p>The owner of the remote repository that contains the job artifacts.</p>
     */
    inline const Aws::String& GetRepositoryOwner() const{ return m_repositoryOwner; }

    /**
     * <p>The owner of the remote repository that contains the job artifacts.</p>
     */
    inline bool RepositoryOwnerHasBeenSet() const { return m_repositoryOwnerHasBeenSet; }

    /**
     * <p>The owner of the remote repository that contains the job artifacts.</p>
     */
    inline void SetRepositoryOwner(const Aws::String& value) { m_repositoryOwnerHasBeenSet = true; m_repositoryOwner = value; }

    /**
     * <p>The owner of the remote repository that contains the job artifacts.</p>
     */
    inline void SetRepositoryOwner(Aws::String&& value) { m_repositoryOwnerHasBeenSet = true; m_repositoryOwner = std::move(value); }

    /**
     * <p>The owner of the remote repository that contains the job artifacts.</p>
     */
    inline void SetRepositoryOwner(const char* value) { m_repositoryOwnerHasBeenSet = true; m_repositoryOwner.assign(value); }

    /**
     * <p>The owner of the remote repository that contains the job artifacts.</p>
     */
    inline UpdateJobFromSourceControlRequest& WithRepositoryOwner(const Aws::String& value) { SetRepositoryOwner(value); return *this;}

    /**
     * <p>The owner of the remote repository that contains the job artifacts.</p>
     */
    inline UpdateJobFromSourceControlRequest& WithRepositoryOwner(Aws::String&& value) { SetRepositoryOwner(std::move(value)); return *this;}

    /**
     * <p>The owner of the remote repository that contains the job artifacts.</p>
     */
    inline UpdateJobFromSourceControlRequest& WithRepositoryOwner(const char* value) { SetRepositoryOwner(value); return *this;}


    /**
     * <p>An optional branch in the remote repository.</p>
     */
    inline const Aws::String& GetBranchName() const{ return m_branchName; }

    /**
     * <p>An optional branch in the remote repository.</p>
     */
    inline bool BranchNameHasBeenSet() const { return m_branchNameHasBeenSet; }

    /**
     * <p>An optional branch in the remote repository.</p>
     */
    inline void SetBranchName(const Aws::String& value) { m_branchNameHasBeenSet = true; m_branchName = value; }

    /**
     * <p>An optional branch in the remote repository.</p>
     */
    inline void SetBranchName(Aws::String&& value) { m_branchNameHasBeenSet = true; m_branchName = std::move(value); }

    /**
     * <p>An optional branch in the remote repository.</p>
     */
    inline void SetBranchName(const char* value) { m_branchNameHasBeenSet = true; m_branchName.assign(value); }

    /**
     * <p>An optional branch in the remote repository.</p>
     */
    inline UpdateJobFromSourceControlRequest& WithBranchName(const Aws::String& value) { SetBranchName(value); return *this;}

    /**
     * <p>An optional branch in the remote repository.</p>
     */
    inline UpdateJobFromSourceControlRequest& WithBranchName(Aws::String&& value) { SetBranchName(std::move(value)); return *this;}

    /**
     * <p>An optional branch in the remote repository.</p>
     */
    inline UpdateJobFromSourceControlRequest& WithBranchName(const char* value) { SetBranchName(value); return *this;}


    /**
     * <p>An optional folder in the remote repository.</p>
     */
    inline const Aws::String& GetFolder() const{ return m_folder; }

    /**
     * <p>An optional folder in the remote repository.</p>
     */
    inline bool FolderHasBeenSet() const { return m_folderHasBeenSet; }

    /**
     * <p>An optional folder in the remote repository.</p>
     */
    inline void SetFolder(const Aws::String& value) { m_folderHasBeenSet = true; m_folder = value; }

    /**
     * <p>An optional folder in the remote repository.</p>
     */
    inline void SetFolder(Aws::String&& value) { m_folderHasBeenSet = true; m_folder = std::move(value); }

    /**
     * <p>An optional folder in the remote repository.</p>
     */
    inline void SetFolder(const char* value) { m_folderHasBeenSet = true; m_folder.assign(value); }

    /**
     * <p>An optional folder in the remote repository.</p>
     */
    inline UpdateJobFromSourceControlRequest& WithFolder(const Aws::String& value) { SetFolder(value); return *this;}

    /**
     * <p>An optional folder in the remote repository.</p>
     */
    inline UpdateJobFromSourceControlRequest& WithFolder(Aws::String&& value) { SetFolder(std::move(value)); return *this;}

    /**
     * <p>An optional folder in the remote repository.</p>
     */
    inline UpdateJobFromSourceControlRequest& WithFolder(const char* value) { SetFolder(value); return *this;}


    /**
     * <p>A commit ID for a commit in the remote repository.</p>
     */
    inline const Aws::String& GetCommitId() const{ return m_commitId; }

    /**
     * <p>A commit ID for a commit in the remote repository.</p>
     */
    inline bool CommitIdHasBeenSet() const { return m_commitIdHasBeenSet; }

    /**
     * <p>A commit ID for a commit in the remote repository.</p>
     */
    inline void SetCommitId(const Aws::String& value) { m_commitIdHasBeenSet = true; m_commitId = value; }

    /**
     * <p>A commit ID for a commit in the remote repository.</p>
     */
    inline void SetCommitId(Aws::String&& value) { m_commitIdHasBeenSet = true; m_commitId = std::move(value); }

    /**
     * <p>A commit ID for a commit in the remote repository.</p>
     */
    inline void SetCommitId(const char* value) { m_commitIdHasBeenSet = true; m_commitId.assign(value); }

    /**
     * <p>A commit ID for a commit in the remote repository.</p>
     */
    inline UpdateJobFromSourceControlRequest& WithCommitId(const Aws::String& value) { SetCommitId(value); return *this;}

    /**
     * <p>A commit ID for a commit in the remote repository.</p>
     */
    inline UpdateJobFromSourceControlRequest& WithCommitId(Aws::String&& value) { SetCommitId(std::move(value)); return *this;}

    /**
     * <p>A commit ID for a commit in the remote repository.</p>
     */
    inline UpdateJobFromSourceControlRequest& WithCommitId(const char* value) { SetCommitId(value); return *this;}


    /**
     * <p>The type of authentication, which can be an authentication token stored in
     * Amazon Web Services Secrets Manager, or a personal access token.</p>
     */
    inline const SourceControlAuthStrategy& GetAuthStrategy() const{ return m_authStrategy; }

    /**
     * <p>The type of authentication, which can be an authentication token stored in
     * Amazon Web Services Secrets Manager, or a personal access token.</p>
     */
    inline bool AuthStrategyHasBeenSet() const { return m_authStrategyHasBeenSet; }

    /**
     * <p>The type of authentication, which can be an authentication token stored in
     * Amazon Web Services Secrets Manager, or a personal access token.</p>
     */
    inline void SetAuthStrategy(const SourceControlAuthStrategy& value) { m_authStrategyHasBeenSet = true; m_authStrategy = value; }

    /**
     * <p>The type of authentication, which can be an authentication token stored in
     * Amazon Web Services Secrets Manager, or a personal access token.</p>
     */
    inline void SetAuthStrategy(SourceControlAuthStrategy&& value) { m_authStrategyHasBeenSet = true; m_authStrategy = std::move(value); }

    /**
     * <p>The type of authentication, which can be an authentication token stored in
     * Amazon Web Services Secrets Manager, or a personal access token.</p>
     */
    inline UpdateJobFromSourceControlRequest& WithAuthStrategy(const SourceControlAuthStrategy& value) { SetAuthStrategy(value); return *this;}

    /**
     * <p>The type of authentication, which can be an authentication token stored in
     * Amazon Web Services Secrets Manager, or a personal access token.</p>
     */
    inline UpdateJobFromSourceControlRequest& WithAuthStrategy(SourceControlAuthStrategy&& value) { SetAuthStrategy(std::move(value)); return *this;}


    /**
     * <p>The value of the authorization token.</p>
     */
    inline const Aws::String& GetAuthToken() const{ return m_authToken; }

    /**
     * <p>The value of the authorization token.</p>
     */
    inline bool AuthTokenHasBeenSet() const { return m_authTokenHasBeenSet; }

    /**
     * <p>The value of the authorization token.</p>
     */
    inline void SetAuthToken(const Aws::String& value) { m_authTokenHasBeenSet = true; m_authToken = value; }

    /**
     * <p>The value of the authorization token.</p>
     */
    inline void SetAuthToken(Aws::String&& value) { m_authTokenHasBeenSet = true; m_authToken = std::move(value); }

    /**
     * <p>The value of the authorization token.</p>
     */
    inline void SetAuthToken(const char* value) { m_authTokenHasBeenSet = true; m_authToken.assign(value); }

    /**
     * <p>The value of the authorization token.</p>
     */
    inline UpdateJobFromSourceControlRequest& WithAuthToken(const Aws::String& value) { SetAuthToken(value); return *this;}

    /**
     * <p>The value of the authorization token.</p>
     */
    inline UpdateJobFromSourceControlRequest& WithAuthToken(Aws::String&& value) { SetAuthToken(std::move(value)); return *this;}

    /**
     * <p>The value of the authorization token.</p>
     */
    inline UpdateJobFromSourceControlRequest& WithAuthToken(const char* value) { SetAuthToken(value); return *this;}

  private:

    Aws::String m_jobName;
    bool m_jobNameHasBeenSet = false;

    SourceControlProvider m_provider;
    bool m_providerHasBeenSet = false;

    Aws::String m_repositoryName;
    bool m_repositoryNameHasBeenSet = false;

    Aws::String m_repositoryOwner;
    bool m_repositoryOwnerHasBeenSet = false;

    Aws::String m_branchName;
    bool m_branchNameHasBeenSet = false;

    Aws::String m_folder;
    bool m_folderHasBeenSet = false;

    Aws::String m_commitId;
    bool m_commitIdHasBeenSet = false;

    SourceControlAuthStrategy m_authStrategy;
    bool m_authStrategyHasBeenSet = false;

    Aws::String m_authToken;
    bool m_authTokenHasBeenSet = false;
  };

} // namespace Model
} // namespace Glue
} // namespace Aws
