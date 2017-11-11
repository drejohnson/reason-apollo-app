open GraphqlTypes;

[@bs.module] external gql : gql = "graphql-tag";

type episode = {. "id": string, "title": string};

type result = {. "episode": episode};

let httpLinkOptions: ApolloClient.linkOptions = {
  "uri": "https://api.graph.cool/simple/v1/PHRESHR"
};

let apolloClientOptions: ApolloClient.clientOptions = {
  "cache": ApolloClient.cache(),
  "link": ApolloClient.httpLink(httpLinkOptions)
};

let client = ApolloClient.apolloClient(apolloClientOptions);

let query =
  [@bs]
  gql(
    {|
  query episode {
    episode: Episode (id: "cixm6eovajibk0180k40rcoja") {
      id
      title
    }
  }
 |}
  );

type action =
  | Result(result)
  | Error(string);

type state = {
  result: episode,
  error: string
};

let component = ReasonReact.reducerComponent("Hello");

let make = (~message, _children) => {
  ...component,
  initialState: () => {result: Js.Obj.empty(), error: ""},
  reducer: (action, state) =>
    switch action {
    /* TODO enhance result */
    | Result(data) => ReasonReact.Update({...state, result: data##episode})
    | Error(error) => ReasonReact.Update({...state, error})
    },
  didMount: ({reduce}) => {
    let _ =
      Js.Promise.(
        /* TODO enhance query */
        resolve(client##query({"query": query}))
        |> then_(
             (response) => {
               let data = response##data;
               reduce(() => Result(data), ());
               resolve()
             }
           )
      );
    ReasonReact.NoUpdate
  },
  render: ({state}) => {
    let episode = state.result;
    <div>
      <div> (ReasonReact.stringToElement(message)) </div>
      <h2> (ReasonReact.stringToElement("Episode!")) </h2>
      <Episode key=episode##id title=episode##title />
    </div>
  }
};