﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/rds/RDS_EXPORTS.h>
#include <aws/rds/RDSRequest.h>
#include <aws/core/utils/memory/stl/AWSString.h>
#include <aws/core/utils/memory/stl/AWSVector.h>
#include <aws/rds/model/Parameter.h>
#include <utility>

namespace Aws
{
namespace RDS
{
namespace Model
{

  /**
   * <p/><p><h3>See Also:</h3>   <a
   * href="http://docs.aws.amazon.com/goto/WebAPI/rds-2014-10-31/ModifyDBClusterParameterGroupMessage">AWS
   * API Reference</a></p>
   */
  class AWS_RDS_API ModifyDBClusterParameterGroupRequest : public RDSRequest
  {
  public:
    ModifyDBClusterParameterGroupRequest();

    /**
     * Virtual Copy Constructor idiom;
     * Override in an inherited class to have overridden functions (such as GetRequestSpecificHeaders) be called by the SDK.
     * (If you are creating a child of this: don't forget to keep the original return type or (better) just use the 'override' keyword)
    */
    virtual Aws::UniquePtr<ModifyDBClusterParameterGroupRequest> Clone() const
    {
      return Aws::MakeUnique<ModifyDBClusterParameterGroupRequest>(GetServiceRequestName(), *this);
    }

    // Service request name is the Operation name which will send this request out,
    // each operation should has unique request name, so that we can get operation's name from this request.
    // Note: this is not true for response, multiple operations may have the same response name,
    // so we can not get operation's name from response.
    inline virtual const char* GetServiceRequestName() const override { return "ModifyDBClusterParameterGroup"; }

    Aws::String SerializePayload() const override;

  protected:
    void DumpBodyToUrl(Aws::Http::URI& uri ) const override;

  public:

    /**
     * <p>The name of the DB cluster parameter group to modify.</p>
     */
    inline const Aws::String& GetDBClusterParameterGroupName() const{ return m_dBClusterParameterGroupName; }

    /**
     * <p>The name of the DB cluster parameter group to modify.</p>
     */
    inline bool DBClusterParameterGroupNameHasBeenSet() const { return m_dBClusterParameterGroupNameHasBeenSet; }

    /**
     * <p>The name of the DB cluster parameter group to modify.</p>
     */
    inline void SetDBClusterParameterGroupName(const Aws::String& value) { m_dBClusterParameterGroupNameHasBeenSet = true; m_dBClusterParameterGroupName = value; }

    /**
     * <p>The name of the DB cluster parameter group to modify.</p>
     */
    inline void SetDBClusterParameterGroupName(Aws::String&& value) { m_dBClusterParameterGroupNameHasBeenSet = true; m_dBClusterParameterGroupName = std::move(value); }

    /**
     * <p>The name of the DB cluster parameter group to modify.</p>
     */
    inline void SetDBClusterParameterGroupName(const char* value) { m_dBClusterParameterGroupNameHasBeenSet = true; m_dBClusterParameterGroupName.assign(value); }

    /**
     * <p>The name of the DB cluster parameter group to modify.</p>
     */
    inline ModifyDBClusterParameterGroupRequest& WithDBClusterParameterGroupName(const Aws::String& value) { SetDBClusterParameterGroupName(value); return *this;}

    /**
     * <p>The name of the DB cluster parameter group to modify.</p>
     */
    inline ModifyDBClusterParameterGroupRequest& WithDBClusterParameterGroupName(Aws::String&& value) { SetDBClusterParameterGroupName(std::move(value)); return *this;}

    /**
     * <p>The name of the DB cluster parameter group to modify.</p>
     */
    inline ModifyDBClusterParameterGroupRequest& WithDBClusterParameterGroupName(const char* value) { SetDBClusterParameterGroupName(value); return *this;}


    /**
     * <p>A list of parameters in the DB cluster parameter group to modify.</p>
     * <p>Valid Values (for the application method): <code>immediate |
     * pending-reboot</code> </p>  <p>You can use the <code>immediate</code>
     * value with dynamic parameters only. You can use the <code>pending-reboot</code>
     * value for both dynamic and static parameters.</p> <p>When the application method
     * is <code>immediate</code>, changes to dynamic parameters are applied immediately
     * to the DB clusters associated with the parameter group. When the application
     * method is <code>pending-reboot</code>, changes to dynamic and static parameters
     * are applied after a reboot without failover to the DB clusters associated with
     * the parameter group.</p> 
     */
    inline const Aws::Vector<Parameter>& GetParameters() const{ return m_parameters; }

    /**
     * <p>A list of parameters in the DB cluster parameter group to modify.</p>
     * <p>Valid Values (for the application method): <code>immediate |
     * pending-reboot</code> </p>  <p>You can use the <code>immediate</code>
     * value with dynamic parameters only. You can use the <code>pending-reboot</code>
     * value for both dynamic and static parameters.</p> <p>When the application method
     * is <code>immediate</code>, changes to dynamic parameters are applied immediately
     * to the DB clusters associated with the parameter group. When the application
     * method is <code>pending-reboot</code>, changes to dynamic and static parameters
     * are applied after a reboot without failover to the DB clusters associated with
     * the parameter group.</p> 
     */
    inline bool ParametersHasBeenSet() const { return m_parametersHasBeenSet; }

    /**
     * <p>A list of parameters in the DB cluster parameter group to modify.</p>
     * <p>Valid Values (for the application method): <code>immediate |
     * pending-reboot</code> </p>  <p>You can use the <code>immediate</code>
     * value with dynamic parameters only. You can use the <code>pending-reboot</code>
     * value for both dynamic and static parameters.</p> <p>When the application method
     * is <code>immediate</code>, changes to dynamic parameters are applied immediately
     * to the DB clusters associated with the parameter group. When the application
     * method is <code>pending-reboot</code>, changes to dynamic and static parameters
     * are applied after a reboot without failover to the DB clusters associated with
     * the parameter group.</p> 
     */
    inline void SetParameters(const Aws::Vector<Parameter>& value) { m_parametersHasBeenSet = true; m_parameters = value; }

    /**
     * <p>A list of parameters in the DB cluster parameter group to modify.</p>
     * <p>Valid Values (for the application method): <code>immediate |
     * pending-reboot</code> </p>  <p>You can use the <code>immediate</code>
     * value with dynamic parameters only. You can use the <code>pending-reboot</code>
     * value for both dynamic and static parameters.</p> <p>When the application method
     * is <code>immediate</code>, changes to dynamic parameters are applied immediately
     * to the DB clusters associated with the parameter group. When the application
     * method is <code>pending-reboot</code>, changes to dynamic and static parameters
     * are applied after a reboot without failover to the DB clusters associated with
     * the parameter group.</p> 
     */
    inline void SetParameters(Aws::Vector<Parameter>&& value) { m_parametersHasBeenSet = true; m_parameters = std::move(value); }

    /**
     * <p>A list of parameters in the DB cluster parameter group to modify.</p>
     * <p>Valid Values (for the application method): <code>immediate |
     * pending-reboot</code> </p>  <p>You can use the <code>immediate</code>
     * value with dynamic parameters only. You can use the <code>pending-reboot</code>
     * value for both dynamic and static parameters.</p> <p>When the application method
     * is <code>immediate</code>, changes to dynamic parameters are applied immediately
     * to the DB clusters associated with the parameter group. When the application
     * method is <code>pending-reboot</code>, changes to dynamic and static parameters
     * are applied after a reboot without failover to the DB clusters associated with
     * the parameter group.</p> 
     */
    inline ModifyDBClusterParameterGroupRequest& WithParameters(const Aws::Vector<Parameter>& value) { SetParameters(value); return *this;}

    /**
     * <p>A list of parameters in the DB cluster parameter group to modify.</p>
     * <p>Valid Values (for the application method): <code>immediate |
     * pending-reboot</code> </p>  <p>You can use the <code>immediate</code>
     * value with dynamic parameters only. You can use the <code>pending-reboot</code>
     * value for both dynamic and static parameters.</p> <p>When the application method
     * is <code>immediate</code>, changes to dynamic parameters are applied immediately
     * to the DB clusters associated with the parameter group. When the application
     * method is <code>pending-reboot</code>, changes to dynamic and static parameters
     * are applied after a reboot without failover to the DB clusters associated with
     * the parameter group.</p> 
     */
    inline ModifyDBClusterParameterGroupRequest& WithParameters(Aws::Vector<Parameter>&& value) { SetParameters(std::move(value)); return *this;}

    /**
     * <p>A list of parameters in the DB cluster parameter group to modify.</p>
     * <p>Valid Values (for the application method): <code>immediate |
     * pending-reboot</code> </p>  <p>You can use the <code>immediate</code>
     * value with dynamic parameters only. You can use the <code>pending-reboot</code>
     * value for both dynamic and static parameters.</p> <p>When the application method
     * is <code>immediate</code>, changes to dynamic parameters are applied immediately
     * to the DB clusters associated with the parameter group. When the application
     * method is <code>pending-reboot</code>, changes to dynamic and static parameters
     * are applied after a reboot without failover to the DB clusters associated with
     * the parameter group.</p> 
     */
    inline ModifyDBClusterParameterGroupRequest& AddParameters(const Parameter& value) { m_parametersHasBeenSet = true; m_parameters.push_back(value); return *this; }

    /**
     * <p>A list of parameters in the DB cluster parameter group to modify.</p>
     * <p>Valid Values (for the application method): <code>immediate |
     * pending-reboot</code> </p>  <p>You can use the <code>immediate</code>
     * value with dynamic parameters only. You can use the <code>pending-reboot</code>
     * value for both dynamic and static parameters.</p> <p>When the application method
     * is <code>immediate</code>, changes to dynamic parameters are applied immediately
     * to the DB clusters associated with the parameter group. When the application
     * method is <code>pending-reboot</code>, changes to dynamic and static parameters
     * are applied after a reboot without failover to the DB clusters associated with
     * the parameter group.</p> 
     */
    inline ModifyDBClusterParameterGroupRequest& AddParameters(Parameter&& value) { m_parametersHasBeenSet = true; m_parameters.push_back(std::move(value)); return *this; }

  private:

    Aws::String m_dBClusterParameterGroupName;
    bool m_dBClusterParameterGroupNameHasBeenSet = false;

    Aws::Vector<Parameter> m_parameters;
    bool m_parametersHasBeenSet = false;
  };

} // namespace Model
} // namespace RDS
} // namespace Aws
