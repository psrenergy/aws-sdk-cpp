﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/appmesh/AppMesh_EXPORTS.h>
#include <aws/core/utils/memory/stl/AWSVector.h>
#include <aws/appmesh/model/WeightedTarget.h>
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
namespace AppMesh
{
namespace Model
{

  /**
   * <p>An object that represents the action to take if a match is
   * determined.</p><p><h3>See Also:</h3>   <a
   * href="http://docs.aws.amazon.com/goto/WebAPI/appmesh-2019-01-25/TcpRouteAction">AWS
   * API Reference</a></p>
   */
  class AWS_APPMESH_API TcpRouteAction
  {
  public:
    TcpRouteAction();
    TcpRouteAction(Aws::Utils::Json::JsonView jsonValue);
    TcpRouteAction& operator=(Aws::Utils::Json::JsonView jsonValue);
    Aws::Utils::Json::JsonValue Jsonize() const;


    /**
     * <p>An object that represents the targets that traffic is routed to when a
     * request matches the route.</p>
     */
    inline const Aws::Vector<WeightedTarget>& GetWeightedTargets() const{ return m_weightedTargets; }

    /**
     * <p>An object that represents the targets that traffic is routed to when a
     * request matches the route.</p>
     */
    inline bool WeightedTargetsHasBeenSet() const { return m_weightedTargetsHasBeenSet; }

    /**
     * <p>An object that represents the targets that traffic is routed to when a
     * request matches the route.</p>
     */
    inline void SetWeightedTargets(const Aws::Vector<WeightedTarget>& value) { m_weightedTargetsHasBeenSet = true; m_weightedTargets = value; }

    /**
     * <p>An object that represents the targets that traffic is routed to when a
     * request matches the route.</p>
     */
    inline void SetWeightedTargets(Aws::Vector<WeightedTarget>&& value) { m_weightedTargetsHasBeenSet = true; m_weightedTargets = std::move(value); }

    /**
     * <p>An object that represents the targets that traffic is routed to when a
     * request matches the route.</p>
     */
    inline TcpRouteAction& WithWeightedTargets(const Aws::Vector<WeightedTarget>& value) { SetWeightedTargets(value); return *this;}

    /**
     * <p>An object that represents the targets that traffic is routed to when a
     * request matches the route.</p>
     */
    inline TcpRouteAction& WithWeightedTargets(Aws::Vector<WeightedTarget>&& value) { SetWeightedTargets(std::move(value)); return *this;}

    /**
     * <p>An object that represents the targets that traffic is routed to when a
     * request matches the route.</p>
     */
    inline TcpRouteAction& AddWeightedTargets(const WeightedTarget& value) { m_weightedTargetsHasBeenSet = true; m_weightedTargets.push_back(value); return *this; }

    /**
     * <p>An object that represents the targets that traffic is routed to when a
     * request matches the route.</p>
     */
    inline TcpRouteAction& AddWeightedTargets(WeightedTarget&& value) { m_weightedTargetsHasBeenSet = true; m_weightedTargets.push_back(std::move(value)); return *this; }

  private:

    Aws::Vector<WeightedTarget> m_weightedTargets;
    bool m_weightedTargetsHasBeenSet = false;
  };

} // namespace Model
} // namespace AppMesh
} // namespace Aws
