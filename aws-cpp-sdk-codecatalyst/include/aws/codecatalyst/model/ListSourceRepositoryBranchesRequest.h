﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/codecatalyst/CodeCatalyst_EXPORTS.h>
#include <aws/codecatalyst/CodeCatalystRequest.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <utility>

namespace Aws
{
namespace CodeCatalyst
{
namespace Model
{

  /**
   */
  class AWS_CODECATALYST_API ListSourceRepositoryBranchesRequest : public CodeCatalystRequest
  {
  public:
    ListSourceRepositoryBranchesRequest();

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "ListSourceRepositoryBranches"; }

    Aws::String SerializePayload() const override;


    /**
     * <p>The name of the space.</p>
     */
    inline const Aws::String& GetSpaceName() const{ return m_spaceName; }

    /**
     * <p>The name of the space.</p>
     */
    inline bool SpaceNameHasBeenSet() const { return m_spaceNameHasBeenSet; }

    /**
     * <p>The name of the space.</p>
     */
    inline void SetSpaceName(const Aws::String& value) { m_spaceNameHasBeenSet = true; m_spaceName = value; }

    /**
     * <p>The name of the space.</p>
     */
    inline void SetSpaceName(Aws::String&& value) { m_spaceNameHasBeenSet = true; m_spaceName = std::move(value); }

    /**
     * <p>The name of the space.</p>
     */
    inline void SetSpaceName(const char* value) { m_spaceNameHasBeenSet = true; m_spaceName.assign(value); }

    /**
     * <p>The name of the space.</p>
     */
    inline ListSourceRepositoryBranchesRequest& WithSpaceName(const Aws::String& value) { SetSpaceName(value); return *this;}

    /**
     * <p>The name of the space.</p>
     */
    inline ListSourceRepositoryBranchesRequest& WithSpaceName(Aws::String&& value) { SetSpaceName(std::move(value)); return *this;}

    /**
     * <p>The name of the space.</p>
     */
    inline ListSourceRepositoryBranchesRequest& WithSpaceName(const char* value) { SetSpaceName(value); return *this;}


    /**
     * <p>The name of the project in the space.</p>
     */
    inline const Aws::String& GetProjectName() const{ return m_projectName; }

    /**
     * <p>The name of the project in the space.</p>
     */
    inline bool ProjectNameHasBeenSet() const { return m_projectNameHasBeenSet; }

    /**
     * <p>The name of the project in the space.</p>
     */
    inline void SetProjectName(const Aws::String& value) { m_projectNameHasBeenSet = true; m_projectName = value; }

    /**
     * <p>The name of the project in the space.</p>
     */
    inline void SetProjectName(Aws::String&& value) { m_projectNameHasBeenSet = true; m_projectName = std::move(value); }

    /**
     * <p>The name of the project in the space.</p>
     */
    inline void SetProjectName(const char* value) { m_projectNameHasBeenSet = true; m_projectName.assign(value); }

    /**
     * <p>The name of the project in the space.</p>
     */
    inline ListSourceRepositoryBranchesRequest& WithProjectName(const Aws::String& value) { SetProjectName(value); return *this;}

    /**
     * <p>The name of the project in the space.</p>
     */
    inline ListSourceRepositoryBranchesRequest& WithProjectName(Aws::String&& value) { SetProjectName(std::move(value)); return *this;}

    /**
     * <p>The name of the project in the space.</p>
     */
    inline ListSourceRepositoryBranchesRequest& WithProjectName(const char* value) { SetProjectName(value); return *this;}


    /**
     * <p>The name of the source repository.</p>
     */
    inline const Aws::String& GetSourceRepositoryName() const{ return m_sourceRepositoryName; }

    /**
     * <p>The name of the source repository.</p>
     */
    inline bool SourceRepositoryNameHasBeenSet() const { return m_sourceRepositoryNameHasBeenSet; }

    /**
     * <p>The name of the source repository.</p>
     */
    inline void SetSourceRepositoryName(const Aws::String& value) { m_sourceRepositoryNameHasBeenSet = true; m_sourceRepositoryName = value; }

    /**
     * <p>The name of the source repository.</p>
     */
    inline void SetSourceRepositoryName(Aws::String&& value) { m_sourceRepositoryNameHasBeenSet = true; m_sourceRepositoryName = std::move(value); }

    /**
     * <p>The name of the source repository.</p>
     */
    inline void SetSourceRepositoryName(const char* value) { m_sourceRepositoryNameHasBeenSet = true; m_sourceRepositoryName.assign(value); }

    /**
     * <p>The name of the source repository.</p>
     */
    inline ListSourceRepositoryBranchesRequest& WithSourceRepositoryName(const Aws::String& value) { SetSourceRepositoryName(value); return *this;}

    /**
     * <p>The name of the source repository.</p>
     */
    inline ListSourceRepositoryBranchesRequest& WithSourceRepositoryName(Aws::String&& value) { SetSourceRepositoryName(std::move(value)); return *this;}

    /**
     * <p>The name of the source repository.</p>
     */
    inline ListSourceRepositoryBranchesRequest& WithSourceRepositoryName(const char* value) { SetSourceRepositoryName(value); return *this;}


    /**
     * <p>A token returned from a call to this API to indicate the next batch of
     * results to return, if any.</p>
     */
    inline const Aws::String& GetNextToken() const{ return m_nextToken; }

    /**
     * <p>A token returned from a call to this API to indicate the next batch of
     * results to return, if any.</p>
     */
    inline bool NextTokenHasBeenSet() const { return m_nextTokenHasBeenSet; }

    /**
     * <p>A token returned from a call to this API to indicate the next batch of
     * results to return, if any.</p>
     */
    inline void SetNextToken(const Aws::String& value) { m_nextTokenHasBeenSet = true; m_nextToken = value; }

    /**
     * <p>A token returned from a call to this API to indicate the next batch of
     * results to return, if any.</p>
     */
    inline void SetNextToken(Aws::String&& value) { m_nextTokenHasBeenSet = true; m_nextToken = std::move(value); }

    /**
     * <p>A token returned from a call to this API to indicate the next batch of
     * results to return, if any.</p>
     */
    inline void SetNextToken(const char* value) { m_nextTokenHasBeenSet = true; m_nextToken.assign(value); }

    /**
     * <p>A token returned from a call to this API to indicate the next batch of
     * results to return, if any.</p>
     */
    inline ListSourceRepositoryBranchesRequest& WithNextToken(const Aws::String& value) { SetNextToken(value); return *this;}

    /**
     * <p>A token returned from a call to this API to indicate the next batch of
     * results to return, if any.</p>
     */
    inline ListSourceRepositoryBranchesRequest& WithNextToken(Aws::String&& value) { SetNextToken(std::move(value)); return *this;}

    /**
     * <p>A token returned from a call to this API to indicate the next batch of
     * results to return, if any.</p>
     */
    inline ListSourceRepositoryBranchesRequest& WithNextToken(const char* value) { SetNextToken(value); return *this;}


    /**
     * <p>The maximum number of results to show in a single call to this API. If the
     * number of results is larger than the number you specified, the response will
     * include a <code>NextToken</code> element, which you can use to obtain additional
     * results.</p>
     */
    inline int GetMaxResults() const{ return m_maxResults; }

    /**
     * <p>The maximum number of results to show in a single call to this API. If the
     * number of results is larger than the number you specified, the response will
     * include a <code>NextToken</code> element, which you can use to obtain additional
     * results.</p>
     */
    inline bool MaxResultsHasBeenSet() const { return m_maxResultsHasBeenSet; }

    /**
     * <p>The maximum number of results to show in a single call to this API. If the
     * number of results is larger than the number you specified, the response will
     * include a <code>NextToken</code> element, which you can use to obtain additional
     * results.</p>
     */
    inline void SetMaxResults(int value) { m_maxResultsHasBeenSet = true; m_maxResults = value; }

    /**
     * <p>The maximum number of results to show in a single call to this API. If the
     * number of results is larger than the number you specified, the response will
     * include a <code>NextToken</code> element, which you can use to obtain additional
     * results.</p>
     */
    inline ListSourceRepositoryBranchesRequest& WithMaxResults(int value) { SetMaxResults(value); return *this;}

  private:

    Aws::String m_spaceName;
    bool m_spaceNameHasBeenSet = false;

    Aws::String m_projectName;
    bool m_projectNameHasBeenSet = false;

    Aws::String m_sourceRepositoryName;
    bool m_sourceRepositoryNameHasBeenSet = false;

    Aws::String m_nextToken;
    bool m_nextTokenHasBeenSet = false;

    int m_maxResults;
    bool m_maxResultsHasBeenSet = false;
  };

} // namespace Model
} // namespace CodeCatalyst
} // namespace Aws
