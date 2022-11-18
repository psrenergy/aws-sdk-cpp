﻿/**
 * Copyright Amazon.com, Inc. or its affiliates. All Rights Reserved.
 * SPDX-License-Identifier: Apache-2.0.
 */

#pragma once
#include <aws/codestar-connections/CodeStarconnections_EXPORTS.h>
#include <aws/core/client/ClientConfiguration.h>
#include <aws/core/client/AWSClient.h>
#include <aws/core/client/AWSAsyncOperationTemplate.h>
#include <aws/core/utils/json/JsonSerializer.h>
#include <aws/codestar-connections/CodeStarconnectionsServiceClientModel.h>
#include <aws/codestar-connections/CodeStarconnectionsLegacyAsyncMacros.h>

namespace Aws
{
namespace CodeStarconnections
{
  /**
   * <fullname>AWS CodeStar Connections</fullname> <p>This AWS CodeStar Connections
   * API Reference provides descriptions and usage examples of the operations and
   * data types for the AWS CodeStar Connections API. You can use the connections API
   * to work with connections and installations.</p> <p> <i>Connections</i> are
   * configurations that you use to connect AWS resources to external code
   * repositories. Each connection is a resource that can be given to services such
   * as CodePipeline to connect to a third-party repository such as Bitbucket. For
   * example, you can add the connection in CodePipeline so that it triggers your
   * pipeline when a code change is made to your third-party code repository. Each
   * connection is named and associated with a unique ARN that is used to reference
   * the connection.</p> <p>When you create a connection, the console initiates a
   * third-party connection handshake. <i>Installations</i> are the apps that are
   * used to conduct this handshake. For example, the installation for the Bitbucket
   * provider type is the Bitbucket app. When you create a connection, you can choose
   * an existing installation or create one.</p> <p>When you want to create a
   * connection to an installed provider type such as GitHub Enterprise Server, you
   * create a <i>host</i> for your connections.</p> <p>You can work with connections
   * by calling:</p> <ul> <li> <p> <a>CreateConnection</a>, which creates a uniquely
   * named connection that can be referenced by services such as CodePipeline.</p>
   * </li> <li> <p> <a>DeleteConnection</a>, which deletes the specified
   * connection.</p> </li> <li> <p> <a>GetConnection</a>, which returns information
   * about the connection, including the connection status.</p> </li> <li> <p>
   * <a>ListConnections</a>, which lists the connections associated with your
   * account.</p> </li> </ul> <p>You can work with hosts by calling:</p> <ul> <li>
   * <p> <a>CreateHost</a>, which creates a host that represents the infrastructure
   * where your provider is installed.</p> </li> <li> <p> <a>DeleteHost</a>, which
   * deletes the specified host.</p> </li> <li> <p> <a>GetHost</a>, which returns
   * information about the host, including the setup status.</p> </li> <li> <p>
   * <a>ListHosts</a>, which lists the hosts associated with your account.</p> </li>
   * </ul> <p>You can work with tags in AWS CodeStar Connections by calling the
   * following:</p> <ul> <li> <p> <a>ListTagsForResource</a>, which gets information
   * about AWS tags for a specified Amazon Resource Name (ARN) in AWS CodeStar
   * Connections.</p> </li> <li> <p> <a>TagResource</a>, which adds or updates tags
   * for a resource in AWS CodeStar Connections.</p> </li> <li> <p>
   * <a>UntagResource</a>, which removes tags for a resource in AWS CodeStar
   * Connections.</p> </li> </ul> <p>For information about how to use AWS CodeStar
   * Connections, see the <a
   * href="https://docs.aws.amazon.com/dtconsole/latest/userguide/welcome-connections.html">Developer
   * Tools User Guide</a>.</p>
   */
  class AWS_CODESTARCONNECTIONS_API CodeStarconnectionsClient : public Aws::Client::AWSJsonClient
  {
    public:
      typedef Aws::Client::AWSJsonClient BASECLASS;
      static const char* SERVICE_NAME;
      static const char* ALLOCATION_TAG;

       /**
        * Initializes client to use DefaultCredentialProviderChain, with default http client factory, and optional client config. If client config
        * is not specified, it will be initialized to default values.
        */
        CodeStarconnectionsClient(const Aws::CodeStarconnections::CodeStarconnectionsClientConfiguration& clientConfiguration = Aws::CodeStarconnections::CodeStarconnectionsClientConfiguration(),
                                  std::shared_ptr<CodeStarconnectionsEndpointProviderBase> endpointProvider = Aws::MakeShared<CodeStarconnectionsEndpointProvider>(ALLOCATION_TAG));

       /**
        * Initializes client to use SimpleAWSCredentialsProvider, with default http client factory, and optional client config. If client config
        * is not specified, it will be initialized to default values.
        */
        CodeStarconnectionsClient(const Aws::Auth::AWSCredentials& credentials,
                                  std::shared_ptr<CodeStarconnectionsEndpointProviderBase> endpointProvider = Aws::MakeShared<CodeStarconnectionsEndpointProvider>(ALLOCATION_TAG),
                                  const Aws::CodeStarconnections::CodeStarconnectionsClientConfiguration& clientConfiguration = Aws::CodeStarconnections::CodeStarconnectionsClientConfiguration());

       /**
        * Initializes client to use specified credentials provider with specified client config. If http client factory is not supplied,
        * the default http client factory will be used
        */
        CodeStarconnectionsClient(const std::shared_ptr<Aws::Auth::AWSCredentialsProvider>& credentialsProvider,
                                  std::shared_ptr<CodeStarconnectionsEndpointProviderBase> endpointProvider = Aws::MakeShared<CodeStarconnectionsEndpointProvider>(ALLOCATION_TAG),
                                  const Aws::CodeStarconnections::CodeStarconnectionsClientConfiguration& clientConfiguration = Aws::CodeStarconnections::CodeStarconnectionsClientConfiguration());


        /* Legacy constructors due deprecation */
       /**
        * Initializes client to use DefaultCredentialProviderChain, with default http client factory, and optional client config. If client config
        * is not specified, it will be initialized to default values.
        */
        CodeStarconnectionsClient(const Aws::Client::ClientConfiguration& clientConfiguration);

       /**
        * Initializes client to use SimpleAWSCredentialsProvider, with default http client factory, and optional client config. If client config
        * is not specified, it will be initialized to default values.
        */
        CodeStarconnectionsClient(const Aws::Auth::AWSCredentials& credentials,
                                  const Aws::Client::ClientConfiguration& clientConfiguration);

       /**
        * Initializes client to use specified credentials provider with specified client config. If http client factory is not supplied,
        * the default http client factory will be used
        */
        CodeStarconnectionsClient(const std::shared_ptr<Aws::Auth::AWSCredentialsProvider>& credentialsProvider,
                                  const Aws::Client::ClientConfiguration& clientConfiguration);

        /* End of legacy constructors due deprecation */
        virtual ~CodeStarconnectionsClient();


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
         * <p>Creates a connection that can then be given to other AWS services like
         * CodePipeline so that it can access third-party code repositories. The connection
         * is in pending status until the third-party connection handshake is completed
         * from the console.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/codestar-connections-2019-12-01/CreateConnection">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateConnectionOutcome CreateConnection(const Model::CreateConnectionRequest& request) const;


        /**
         * <p>Creates a resource that represents the infrastructure where a third-party
         * provider is installed. The host is used when you create connections to an
         * installed third-party provider type, such as GitHub Enterprise Server. You
         * create one host for all connections to that provider.</p>  <p>A host
         * created through the CLI or the SDK is in `PENDING` status by default. You can
         * make its status `AVAILABLE` by setting up the host in the console.</p>
         * <p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/codestar-connections-2019-12-01/CreateHost">AWS
         * API Reference</a></p>
         */
        virtual Model::CreateHostOutcome CreateHost(const Model::CreateHostRequest& request) const;


        /**
         * <p>The connection to be deleted.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/codestar-connections-2019-12-01/DeleteConnection">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteConnectionOutcome DeleteConnection(const Model::DeleteConnectionRequest& request) const;


        /**
         * <p>The host to be deleted. Before you delete a host, all connections associated
         * to the host must be deleted.</p>  <p>A host cannot be deleted if it is in
         * the VPC_CONFIG_INITIALIZING or VPC_CONFIG_DELETING state.</p> <p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/codestar-connections-2019-12-01/DeleteHost">AWS
         * API Reference</a></p>
         */
        virtual Model::DeleteHostOutcome DeleteHost(const Model::DeleteHostRequest& request) const;


        /**
         * <p>Returns the connection ARN and details such as status, owner, and provider
         * type.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/codestar-connections-2019-12-01/GetConnection">AWS
         * API Reference</a></p>
         */
        virtual Model::GetConnectionOutcome GetConnection(const Model::GetConnectionRequest& request) const;


        /**
         * <p>Returns the host ARN and details such as status, provider type, endpoint,
         * and, if applicable, the VPC configuration.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/codestar-connections-2019-12-01/GetHost">AWS
         * API Reference</a></p>
         */
        virtual Model::GetHostOutcome GetHost(const Model::GetHostRequest& request) const;


        /**
         * <p>Lists the connections associated with your account.</p><p><h3>See Also:</h3> 
         * <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/codestar-connections-2019-12-01/ListConnections">AWS
         * API Reference</a></p>
         */
        virtual Model::ListConnectionsOutcome ListConnections(const Model::ListConnectionsRequest& request) const;


        /**
         * <p>Lists the hosts associated with your account.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/codestar-connections-2019-12-01/ListHosts">AWS
         * API Reference</a></p>
         */
        virtual Model::ListHostsOutcome ListHosts(const Model::ListHostsRequest& request) const;


        /**
         * <p>Gets the set of key-value pairs (metadata) that are used to manage the
         * resource.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/codestar-connections-2019-12-01/ListTagsForResource">AWS
         * API Reference</a></p>
         */
        virtual Model::ListTagsForResourceOutcome ListTagsForResource(const Model::ListTagsForResourceRequest& request) const;


        /**
         * <p>Adds to or modifies the tags of the given resource. Tags are metadata that
         * can be used to manage a resource.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/codestar-connections-2019-12-01/TagResource">AWS
         * API Reference</a></p>
         */
        virtual Model::TagResourceOutcome TagResource(const Model::TagResourceRequest& request) const;


        /**
         * <p>Removes tags from an AWS resource.</p><p><h3>See Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/codestar-connections-2019-12-01/UntagResource">AWS
         * API Reference</a></p>
         */
        virtual Model::UntagResourceOutcome UntagResource(const Model::UntagResourceRequest& request) const;


        /**
         * <p>Updates a specified host with the provided configurations.</p><p><h3>See
         * Also:</h3>   <a
         * href="http://docs.aws.amazon.com/goto/WebAPI/codestar-connections-2019-12-01/UpdateHost">AWS
         * API Reference</a></p>
         */
        virtual Model::UpdateHostOutcome UpdateHost(const Model::UpdateHostRequest& request) const;



      void OverrideEndpoint(const Aws::String& endpoint);
      std::shared_ptr<CodeStarconnectionsEndpointProviderBase>& accessEndpointProvider();
    private:
      void init(const CodeStarconnectionsClientConfiguration& clientConfiguration);

      CodeStarconnectionsClientConfiguration m_clientConfiguration;
      std::shared_ptr<Aws::Utils::Threading::Executor> m_executor;
      std::shared_ptr<CodeStarconnectionsEndpointProviderBase> m_endpointProvider;
  };

} // namespace CodeStarconnections
} // namespace Aws
