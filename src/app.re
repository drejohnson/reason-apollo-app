open GraphqlTypes;

[%bs.raw {|require('./app.css')|}];

[@bs.module] external logo : string = "./logo.svg";

[@bs.module] external gql : gql = "graphql-tag";

type episode = {. "id": string, "title": string};

type load = {. "allEpisodes": array(episode)};

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
  | Load(load)
  | Error(string);

type state = {
  episodes: array(episode),
  error: string
};

let component = ReasonReact.reducerComponent("App");

let make = (~message, _children) => {
  ...component,
  initialState: () => {episodes: [||], error: ""},
  reducer: (action, state) =>
    switch action {
    | Load(data) =>
      let allEpisodes = data##allEpisodes;
      ReasonReact.Update({...state, episodes: allEpisodes})
    | Error(error) => ReasonReact.Update({...state, error})
    },
  didMount: ({reduce}) => {
    let _ =
      Js.Promise.(
        resolve(client##query({"query": query}))
        |> then_(
             (response) => {
               let responseData = response##data;
               let loadAction = (responseData) => Load(responseData);
               reduce(loadAction, responseData);
               resolve()
             }
           )
      );
    ReasonReact.NoUpdate
  },
  render: ({state}) => {
    let episodeItems =
      Array.map((episode) => <Episode key=episode##id title=episode##title />, state.episodes);
    <div className="App">
      <div className="App-header">
        <img src=logo className="App-logo" alt="logo" />
        <h2> (ReasonReact.stringToElement(message)) </h2>
      </div>
      <h2> (ReasonReact.stringToElement("Episodes!")) </h2>
      (ReasonReact.arrayToElement(episodeItems))
    </div>
  }
};