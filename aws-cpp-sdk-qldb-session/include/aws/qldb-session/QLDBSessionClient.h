﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/qldb-session/QLDBSession_EXPORTS.h>
#include <aws/core/client/ClientConfiguration.h>
#include <aws/core/client/AWSClient.h>
#include <aws/core/client/AWSAsyncOperationTemplate.h>
#include <aws/core/utils/json/JsonSerializer.h>
#include <aws/qldb-session/QLDBSessionServiceClientModel.h>
#include <aws/qldb-session/QLDBSessionLegacyAsyncMacros.h>

namespace Aws
{
namespace QLDBSession
{
  /**
   * <p>The transactional data APIs for Amazon QLDB</p>  <p>Instead of
   * interacting directly with this API, we recommend using the QLDB driver or the
   * QLDB shell to execute data transactions on a ledger.</p> <ul> <li> <p>If you are
   * working with an AWS SDK, use the QLDB driver. The driver provides a high-level
   * abstraction layer above this <i>QLDB Session</i> data plane and manages
   * <code>SendCommand</code> API calls for you. For information and a list of
   * supported programming languages, see <a
   * href="https://docs.aws.amazon.com/qldb/latest/developerguide/getting-started-driver.html">Getting
   * started with the driver</a> in the <i>Amazon QLDB Developer Guide</i>.</p> </li>
   * <li> <p>If you are working with the AWS Command Line Interface (AWS CLI), use
   * the QLDB shell. The shell is a command line interface that uses the QLDB driver
   * to interact with a ledger. For information, see <a
   * href="https://docs.aws.amazon.com/qldb/latest/developerguide/data-shell.html">Accessing
   * Amazon QLDB using the QLDB shell</a>.</p> </li> </ul> 
   */
  class AWS_QLDBSESSION_API QLDBSessionClient : public Aws::Client::AWSJsonClient
  {
    public:
      typedef Aws::Client::AWSJsonClient BASECLASS;
      static const char* SERVICE_NAME;
      static const char* ALLOCATION_TAG;

       /**
        * Initializes client to use DefaultCredentialProviderChain, with default http client factory, and optional client config. If client config
        * is not specified, it will be initialized to default values.
        */
        QLDBSessionClient(const Aws::QLDBSession::QLDBSessionClientConfiguration& clientConfiguration = Aws::QLDBSession::QLDBSessionClientConfiguration(),
                          std::shared_ptr<QLDBSessionEndpointProviderBase> endpointProvider = Aws::MakeShared<QLDBSessionEndpointProvider>(ALLOCATION_TAG));

       /**
        * Initializes client to use SimpleAWSCredentialsProvider, with default http client factory, and optional client config. If client config
        * is not specified, it will be initialized to default values.
        */
        QLDBSessionClient(const Aws::Auth::AWSCredentials& credentials,
                          std::shared_ptr<QLDBSessionEndpointProviderBase> endpointProvider = Aws::MakeShared<QLDBSessionEndpointProvider>(ALLOCATION_TAG),
                          const Aws::QLDBSession::QLDBSessionClientConfiguration& clientConfiguration = Aws::QLDBSession::QLDBSessionClientConfiguration());

       /**
        * Initializes client to use specified credentials provider with specified client config. If http client factory is not supplied,
        * the default http client factory will be used
        */
        QLDBSessionClient(const std::shared_ptr<Aws::Auth::AWSCredentialsProvider>& credentialsProvider,
                          std::shared_ptr<QLDBSessionEndpointProviderBase> endpointProvider = Aws::MakeShared<QLDBSessionEndpointProvider>(ALLOCATION_TAG),
                          const Aws::QLDBSession::QLDBSessionClientConfiguration& clientConfiguration = Aws::QLDBSession::QLDBSessionClientConfiguration());


        /* Legacy constructors due deprecation */
       /**
        * Initializes client to use DefaultCredentialProviderChain, with default http client factory, and optional client config. If client config
        * is not specified, it will be initialized to default values.
        */
        QLDBSessionClient(const Aws::Client::ClientConfiguration& clientConfiguration);

       /**
        * Initializes client to use SimpleAWSCredentialsProvider, with default http client factory, and optional client config. If client config
        * is not specified, it will be initialized to default values.
        */
        QLDBSessionClient(const Aws::Auth::AWSCredentials& credentials,
                          const Aws::Client::ClientConfiguration& clientConfiguration);

       /**
        * Initializes client to use specified credentials provider with specified client config. If http client factory is not supplied,
        * the default http client factory will be used
        */
        QLDBSessionClient(const std::shared_ptr<Aws::Auth::AWSCredentialsProvider>& credentialsProvider,
                          const Aws::Client::ClientConfiguration& clientConfiguration);

        /* End of legacy constructors due deprecation */
        virtual ~QLDBSessionClient();


        template<typename RequestT,
                 typename HandlerT,
                 typename HandlerContextT,
                 typename OperationFuncT>
        void SubmitAsync(OperationFuncT&& operationFunc,
                         const RequestT& request,
                         const HandlerT& handler,
                         const HandlerContextT& context)
        {
            Aws::Client::MakeAsyncOperation(std::forward<OperationFuncT>(operationFunc), this, request, handler, context, m_executor.get());
        }

        template<typename RequestT,
                 typename HandlerT,
                 typename HandlerContextT,
                 typename OperationFuncT>
        void SubmitAsync(OperationFuncT&& operationFunc,
                         RequestT& request,
                         const HandlerT& handler,
                         const HandlerContextT& context)
        {
            Aws::Client::MakeAsyncStreamingOperation(std::forward<OperationFuncT>(operationFunc), this, request, handler, context, m_executor.get());
        }

        template<typename RequestT,
                 typename OperationFuncT>
        auto SubmitCallable(OperationFuncT&& operationFunc,
                            const RequestT& request) -> std::future<decltype((this->*operationFunc)(request))>
        {
            return Aws::Client::MakeCallableOperation(ALLOCATION_TAG, operationFunc, this, request, m_executor.get());
        }

        template<typename RequestT,
                 typename OperationFuncT>
        auto SubmitCallable(OperationFuncT&& operationFunc,
                            RequestT& request) -> std::future<decltype((this->*operationFunc)(request))>
        {
            return Aws::Client::MakeCallableStreamingOperation(ALLOCATION_TAG, operationFunc, this, request, m_executor.get());
        }


        /**
         * <p>Sends a command to an Amazon QLDB ledger.</p>  <p>Instead of
         * interacting directly with this API, we recommend using the QLDB driver or the
         * QLDB shell to execute data transactions on a ledger.</p> <ul> <li> <p>If you are
         * working with an AWS SDK, use the QLDB driver. The driver provides a high-level
         * abstraction layer above this <i>QLDB Session</i> data plane and manages
         * <code>SendCommand</code> API calls for you. For information and a list of
         * supported programming languages, see <a
         * href="https://docs.aws.amazon.com/qldb/latest/developerguide/getting-started-driver.html">Getting
         * started with the driver</a> in the <i>Amazon QLDB Developer Guide</i>.</p> </li>
         * <li> <p>If you are working with the AWS Command Line Interface (AWS CLI), use
         * the QLDB shell. The shell is a command line interface that uses the QLDB driver
         * to interact with a ledger. For information, see <a
         * href="https://docs.aws.amazon.com/qldb/latest/developerguide/data-shell.html">Accessing
         * Amazon QLDB using the QLDB shell</a>.</p> </li> </ul> <p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/qldb-session-2019-07-11/SendCommand">AWS
         * API Reference</a></p>
         */
        virtual Model::SendCommandOutcome SendCommand(const Model::SendCommandRequest& request) const;



      void OverrideEndpoint(const Aws::String& endpoint);
      std::shared_ptr<QLDBSessionEndpointProviderBase>& accessEndpointProvider();
    private:
      void init(const QLDBSessionClientConfiguration& clientConfiguration);

      QLDBSessionClientConfiguration m_clientConfiguration;
      std::shared_ptr<Aws::Utils::Threading::Executor> m_executor;
      std::shared_ptr<QLDBSessionEndpointProviderBase> m_endpointProvider;
  };

} // namespace QLDBSession
} // namespace Aws
