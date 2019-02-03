<template>
    <Page>
        <ActionBar title="Welcome to NativeScript-Vue!"/>

        <GridLayout columns="*" rows="100, 100, 100" width="300" height="600" backgroundColor="lightgray" >
          <Label class="message" :text="msg" col="0" row="0"/>
          <TextField class="message" col="0" row="1" v-model="inputTemp" />
          <Label class="message" :text="inputTemp" col="0" row="2"/>
          <Button text="Button" @tap="add" />
        </GridLayout>
    </Page>
</template>

<script>
  export default {
    data() {
      return {
        msg: 'Hello World!',
        inputTemp: 22
      }
    },

    methods: {
      add () {
        this.inputTemp += 1
      }
    }
  }
</script>

<style scoped>
  ActionBar {
    background-color: #53ba82;
    color: #ffffff;
  }

  .message {
    vertical-align: center;
    text-align: center;
    font-size: 20;
    color: #333333;
  }
</style>
