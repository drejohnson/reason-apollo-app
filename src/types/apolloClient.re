open GraphqlTypes;

type clientOptions = {. "cache": unit, "link": unit};

type linkOptions = {. "uri": string};

type restoreObj = {. "restore": [@bs.meth] (unit => unit)};

[@bs.module "apollo-client"] [@bs.new] external apolloClient : clientOptions => 't =
  "ApolloClient";

[@bs.module "apollo-link-http"] [@bs.new] external httpLink : linkOptions => 'a = "HttpLink";

[@bs.module "apollo-cache-inmemory"] [@bs.new] external cacheRestore : unit => restoreObj =
  "InMemoryCache";

[@bs.module "apollo-cache-inmemory"] [@bs.new] external cache : unit => 'a = "InMemoryCache";

[@bs.module "react-apollo"] [@bs.new] external graphql : query => 'a = "graphql";