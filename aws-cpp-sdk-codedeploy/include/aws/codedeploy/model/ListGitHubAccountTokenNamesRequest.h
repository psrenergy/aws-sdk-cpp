﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/codedeploy/CodeDeploy_EXPORTS.h>
#include <aws/codedeploy/CodeDeployRequest.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <utility>

namespace Aws
{
namespace CodeDeploy
{
namespace Model
{

  /**
   * <p>Represents the input of a <code>ListGitHubAccountTokenNames</code>
   * operation.</p><p><h3>See Also:</h3>   <a
   * href="http://docs.aws.amazon.com/goto/WebAPI/codedeploy-2014-10-06/ListGitHubAccountTokenNamesInput">AWS
   * API Reference</a></p>
   */
  class AWS_CODEDEPLOY_API ListGitHubAccountTokenNamesRequest : public CodeDeployRequest
  {
  public:
    ListGitHubAccountTokenNamesRequest();

    /**
     * Virtual Copy Constructor idiom;
     * Override in an inherited class to have overridden functions (such as GetRequestSpecificHeaders) be called by the SDK.
     * (If you are creating a child of this: don't forget to keep the original return type or (better) just use the 'override' keyword)
    */
    virtual Aws::UniquePtr<ListGitHubAccountTokenNamesRequest> Clone() const
    {
      return Aws::MakeUnique<ListGitHubAccountTokenNamesRequest>(GetServiceRequestName(), *this);
    }

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "ListGitHubAccountTokenNames"; }

    Aws::String SerializePayload() const override;

    Aws::Http::HeaderValueCollection GetRequestSpecificHeaders() const override;


    /**
     * <p>An identifier returned from the previous
     * <code>ListGitHubAccountTokenNames</code> call. It can be used to return the next
     * set of names in the list. </p>
     */
    inline const Aws::String& GetNextToken() const{ return m_nextToken; }

    /**
     * <p>An identifier returned from the previous
     * <code>ListGitHubAccountTokenNames</code> call. It can be used to return the next
     * set of names in the list. </p>
     */
    inline bool NextTokenHasBeenSet() const { return m_nextTokenHasBeenSet; }

    /**
     * <p>An identifier returned from the previous
     * <code>ListGitHubAccountTokenNames</code> call. It can be used to return the next
     * set of names in the list. </p>
     */
    inline void SetNextToken(const Aws::String& value) { m_nextTokenHasBeenSet = true; m_nextToken = value; }

    /**
     * <p>An identifier returned from the previous
     * <code>ListGitHubAccountTokenNames</code> call. It can be used to return the next
     * set of names in the list. </p>
     */
    inline void SetNextToken(Aws::String&& value) { m_nextTokenHasBeenSet = true; m_nextToken = std::move(value); }

    /**
     * <p>An identifier returned from the previous
     * <code>ListGitHubAccountTokenNames</code> call. It can be used to return the next
     * set of names in the list. </p>
     */
    inline void SetNextToken(const char* value) { m_nextTokenHasBeenSet = true; m_nextToken.assign(value); }

    /**
     * <p>An identifier returned from the previous
     * <code>ListGitHubAccountTokenNames</code> call. It can be used to return the next
     * set of names in the list. </p>
     */
    inline ListGitHubAccountTokenNamesRequest& WithNextToken(const Aws::String& value) { SetNextToken(value); return *this;}

    /**
     * <p>An identifier returned from the previous
     * <code>ListGitHubAccountTokenNames</code> call. It can be used to return the next
     * set of names in the list. </p>
     */
    inline ListGitHubAccountTokenNamesRequest& WithNextToken(Aws::String&& value) { SetNextToken(std::move(value)); return *this;}

    /**
     * <p>An identifier returned from the previous
     * <code>ListGitHubAccountTokenNames</code> call. It can be used to return the next
     * set of names in the list. </p>
     */
    inline ListGitHubAccountTokenNamesRequest& WithNextToken(const char* value) { SetNextToken(value); return *this;}

  private:

    Aws::String m_nextToken;
    bool m_nextTokenHasBeenSet = false;
  };

} // namespace Model
} // namespace CodeDeploy
} // namespace Aws
