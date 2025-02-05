﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/codecatalyst/CodeCatalyst_EXPORTS.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <utility>

namespace Aws
{
namespace Utils
{
namespace Json
{
  class JsonValue;
  class JsonView;
} // namespace Json
} // namespace Utils
namespace CodeCatalyst
{
namespace Model
{

  /**
   * <p>Information about the source repsitory for a Dev Environment. </p><p><h3>See
   * Also:</h3>   <a
   * href="http://docs.aws.amazon.com/goto/WebAPI/codecatalyst-2022-09-28/DevEnvironmentRepositorySummary">AWS
   * API Reference</a></p>
   */
  class AWS_CODECATALYST_API DevEnvironmentRepositorySummary
  {
  public:
    DevEnvironmentRepositorySummary();
    DevEnvironmentRepositorySummary(Aws::Utils::Json::JsonView jsonValue);
    DevEnvironmentRepositorySummary& operator=(Aws::Utils::Json::JsonView jsonValue);
    Aws::Utils::Json::JsonValue Jsonize() const;


    /**
     * <p>The name of the source repository.</p>
     */
    inline const Aws::String& GetRepositoryName() const{ return m_repositoryName; }

    /**
     * <p>The name of the source repository.</p>
     */
    inline bool RepositoryNameHasBeenSet() const { return m_repositoryNameHasBeenSet; }

    /**
     * <p>The name of the source repository.</p>
     */
    inline void SetRepositoryName(const Aws::String& value) { m_repositoryNameHasBeenSet = true; m_repositoryName = value; }

    /**
     * <p>The name of the source repository.</p>
     */
    inline void SetRepositoryName(Aws::String&& value) { m_repositoryNameHasBeenSet = true; m_repositoryName = std::move(value); }

    /**
     * <p>The name of the source repository.</p>
     */
    inline void SetRepositoryName(const char* value) { m_repositoryNameHasBeenSet = true; m_repositoryName.assign(value); }

    /**
     * <p>The name of the source repository.</p>
     */
    inline DevEnvironmentRepositorySummary& WithRepositoryName(const Aws::String& value) { SetRepositoryName(value); return *this;}

    /**
     * <p>The name of the source repository.</p>
     */
    inline DevEnvironmentRepositorySummary& WithRepositoryName(Aws::String&& value) { SetRepositoryName(std::move(value)); return *this;}

    /**
     * <p>The name of the source repository.</p>
     */
    inline DevEnvironmentRepositorySummary& WithRepositoryName(const char* value) { SetRepositoryName(value); return *this;}


    /**
     * <p>The name of the branch in a source repository cloned into the Dev
     * Environment. </p>
     */
    inline const Aws::String& GetBranchName() const{ return m_branchName; }

    /**
     * <p>The name of the branch in a source repository cloned into the Dev
     * Environment. </p>
     */
    inline bool BranchNameHasBeenSet() const { return m_branchNameHasBeenSet; }

    /**
     * <p>The name of the branch in a source repository cloned into the Dev
     * Environment. </p>
     */
    inline void SetBranchName(const Aws::String& value) { m_branchNameHasBeenSet = true; m_branchName = value; }

    /**
     * <p>The name of the branch in a source repository cloned into the Dev
     * Environment. </p>
     */
    inline void SetBranchName(Aws::String&& value) { m_branchNameHasBeenSet = true; m_branchName = std::move(value); }

    /**
     * <p>The name of the branch in a source repository cloned into the Dev
     * Environment. </p>
     */
    inline void SetBranchName(const char* value) { m_branchNameHasBeenSet = true; m_branchName.assign(value); }

    /**
     * <p>The name of the branch in a source repository cloned into the Dev
     * Environment. </p>
     */
    inline DevEnvironmentRepositorySummary& WithBranchName(const Aws::String& value) { SetBranchName(value); return *this;}

    /**
     * <p>The name of the branch in a source repository cloned into the Dev
     * Environment. </p>
     */
    inline DevEnvironmentRepositorySummary& WithBranchName(Aws::String&& value) { SetBranchName(std::move(value)); return *this;}

    /**
     * <p>The name of the branch in a source repository cloned into the Dev
     * Environment. </p>
     */
    inline DevEnvironmentRepositorySummary& WithBranchName(const char* value) { SetBranchName(value); return *this;}

  private:

    Aws::String m_repositoryName;
    bool m_repositoryNameHasBeenSet = false;

    Aws::String m_branchName;
    bool m_branchNameHasBeenSet = false;
  };

} // namespace Model
} // namespace CodeCatalyst
} // namespace Aws
