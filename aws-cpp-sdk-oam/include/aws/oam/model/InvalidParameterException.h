﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/oam/OAM_EXPORTS.h>
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
namespace OAM
{
namespace Model
{

  /**
   * <p>A parameter is specified incorrectly.</p><p><h3>See Also:</h3>   <a
   * href="http://docs.aws.amazon.com/goto/WebAPI/oam-2022-06-10/InvalidParameterException">AWS
   * API Reference</a></p>
   */
  class AWS_OAM_API InvalidParameterException
  {
  public:
    InvalidParameterException();
    InvalidParameterException(Aws::Utils::Json::JsonView jsonValue);
    InvalidParameterException& operator=(Aws::Utils::Json::JsonView jsonValue);
    Aws::Utils::Json::JsonValue Jsonize() const;


    
    inline const Aws::String& GetMessage() const{ return m_message; }

    
    inline bool MessageHasBeenSet() const { return m_messageHasBeenSet; }

    
    inline void SetMessage(const Aws::String& value) { m_messageHasBeenSet = true; m_message = value; }

    
    inline void SetMessage(Aws::String&& value) { m_messageHasBeenSet = true; m_message = std::move(value); }

    
    inline void SetMessage(const char* value) { m_messageHasBeenSet = true; m_message.assign(value); }

    
    inline InvalidParameterException& WithMessage(const Aws::String& value) { SetMessage(value); return *this;}

    
    inline InvalidParameterException& WithMessage(Aws::String&& value) { SetMessage(std::move(value)); return *this;}

    
    inline InvalidParameterException& WithMessage(const char* value) { SetMessage(value); return *this;}


    /**
     * <p>The name of the exception.</p>
     */
    inline const Aws::String& GetAmznErrorType() const{ return m_amznErrorType; }

    /**
     * <p>The name of the exception.</p>
     */
    inline bool AmznErrorTypeHasBeenSet() const { return m_amznErrorTypeHasBeenSet; }

    /**
     * <p>The name of the exception.</p>
     */
    inline void SetAmznErrorType(const Aws::String& value) { m_amznErrorTypeHasBeenSet = true; m_amznErrorType = value; }

    /**
     * <p>The name of the exception.</p>
     */
    inline void SetAmznErrorType(Aws::String&& value) { m_amznErrorTypeHasBeenSet = true; m_amznErrorType = std::move(value); }

    /**
     * <p>The name of the exception.</p>
     */
    inline void SetAmznErrorType(const char* value) { m_amznErrorTypeHasBeenSet = true; m_amznErrorType.assign(value); }

    /**
     * <p>The name of the exception.</p>
     */
    inline InvalidParameterException& WithAmznErrorType(const Aws::String& value) { SetAmznErrorType(value); return *this;}

    /**
     * <p>The name of the exception.</p>
     */
    inline InvalidParameterException& WithAmznErrorType(Aws::String&& value) { SetAmznErrorType(std::move(value)); return *this;}

    /**
     * <p>The name of the exception.</p>
     */
    inline InvalidParameterException& WithAmznErrorType(const char* value) { SetAmznErrorType(value); return *this;}

  private:

    Aws::String m_message;
    bool m_messageHasBeenSet = false;

    Aws::String m_amznErrorType;
    bool m_amznErrorTypeHasBeenSet = false;
  };

} // namespace Model
} // namespace OAM
} // namespace Aws
