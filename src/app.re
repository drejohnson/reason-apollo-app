open GraphqlTypes;

[%bs.raw {|require('./app.css')|}];

[@bs.module] external logo : string = "./logo.svg";

[@bs.module] external gql : gql = "graphql-tag";

type episode = {. "id": string, "title": string};

type result = {. "allEpisodes": array(episode)};

let httpLinkOptions: ApolloClient.linkOptions = {
  "uri": "https://api.graph.cool/simple/v1/PHRESHR"
};

let apolloClientOptions: ApolloClient.clientOptions = {
  "cache": ApolloClient.cache(),
  "link": ApolloClient.httpLink(httpLinkOptions)
};

let client = ApolloClient.apolloClient(apolloClientOptions);

let query =
  [@bs] gql({|
  query allEpisodes {
    allEpisodes {
      id
      title
    }
  }
 |});

type action =
  | Result(result)
  | Error(string);

type state = {
  result: array(episode),
  error: string
};

let component = ReasonReact.reducerComponent("App");

let make = (~message, _children) => {
  ...component,
  initialState: () => {result: [||], error: ""},
  reducer: (action, state) =>
    switch action {
    /* TODO enhance result */
    | Result(data) => ReasonReact.Update({...state, result: data##allEpisodes})
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
    let episodes =
      Array.map((episode) => <Episode key=episode##id title=episode##title />, state.result);
    <div className="App">
      <div className="App-header">
        <img src=logo className="App-logo" alt="logo" />
        <h2> (ReasonReact.stringToElement(message)) </h2>
      </div>
      <h2> (ReasonReact.stringToElement("Episodes!")) </h2>
      (ReasonReact.arrayToElement(episodes))
      <Hello message="Hello" />
    </div>
  }
};